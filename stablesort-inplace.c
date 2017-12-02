/*++
    Stable Sort for Random-Access Ranges
        
    Authors:  Xinok
    License:  Public Domain
    
    Bugs:
    Parallel sort fails to compile in debug builds
++*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     

/*++
    Performs a stable sort on a random-access p_aRange according to predicate less.
    The algorithm is a natural merge sort with O(log n log n) space complexity.
    
    Returns: Sorted input as SortedRange
    
    Params:
    p_aRange = Range to be sorted
    p_bThreaded = Set to true for concurrent sorting
    temp = Optionally provide your own additional space for sorting
    
    Params:
    less = Predicate (string, function, or delegate) used for comparing elements; Defaults to "a < b"
    gbInPlace = Set to true to perform an in-place sort using minimal additional space
    R = Type of p_aRange to be sorted; Must be a finite random-access p_aRange with slicing
    
    Examples:
    -----------------
    int[] array = [10, 37, 74, 99, 86, 28, 17, 39, 18, 38, 70];
    stableSort(array);
    stableSort!"a > b"(array); // Sorts array descending
    stableSort(array, true)    // Sorts array using multiple threads
    
    stableSort!("a < b", true)(array);       // Sorts array in-place
    stableSort!("a < b", true)(array, true); // Sorts array in-place using multiple threads
    
    int[] temp;
    temp.length = 64;
    stableSort(array, false, temp); // Sorts array using temporary memory provided by user
    -----------------
++*/


enum {
      MIN_RUN    = 32   // (=2^5)  Maximum length for an insertion sort 
    , MAX_STACK  = 1024 // (=2^10) Maximum number of bytes to allocate on stack
    , MIN_THREAD = 1024 // (=2^10) Minimum length of a sublist to initiate new thread
    , IN_PLACE   = (MIN_RUN * (MIN_RUN - 1)) / 2 // Calculate max number of insertions for in-place merge
};

#define StableSort_Inplace(   _Array, _nLength, ...) csname2(StableSort_Inplace_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define StableSort_Inplace_0( _Array, _nLength)                        _QuickSort3way( _Array, _nLength, ASC )
#define StableSort_Inplace_1( _Array, _nLength, _bIsAscending)         _QuickSort3way( _Array, _nLength, (_bIsAscending) )
#define StableSort_Inplace_2( _Array, _nLength, _Type, _bIsAscending)  _QuickSort3way( _Array, _nLength, (_bIsAscending) )


/// Stable Sort implemenation
    
/// A simple insertion sort used for sorting small sublists
void binaryInsertionSort(long* p_pArray, size_t p_nLen
                         , size_t p_nStarIn   // must start in 1.
                        ) { 
    size_t nLower
         , nUpper
         , nMiddle ;
    long  datum;
    for(; p_nStarIn < p_nLen; ++p_nStarIn) {
        datum  = p_pArray[p_nStarIn];
        nLower = 0;
        nUpper = p_nStarIn;
        while( nUpper != nLower ) {
            nMiddle = (nLower + nUpper) / 2 ;
            if( datum < p_pArray[nMiddle] )
                nUpper = nMiddle;
            else 
                nLower = nMiddle + 1;
        }
        for( nUpper = p_nStarIn ; nLower < nUpper ; --nUpper ) 
            p_pArray[nUpper] = p_pArray[nUpper - 1];
        p_pArray[nUpper] = datum ;
    }
}

#define Log(a)      log10(a)
#define Ln(a)       log(a)
#define ln(a)       log(a)
#define Lg(a)       (log(a)/log(2))
#define lg(a)       (log(a)/log(2))
#define pow2( a )   ( (a) * (a) )

/// Entry sort function
void sort(long*  p_aRange, size_t p_nSlots )    {
    if( p_nSlots <= MIN_RUN)   {
        binaryInsertionSort(p_aRange, p_nSlots, 1);
        return;
    }
    
    // Allocate temporary memory
    long* aTemp;
    size_t nLenTmp;
    
    nLenTmp = (size_t)pow2( lg(p_nSlots) ) ;
    aTemp   = New( long,  nLenTmp) ;
    
    sort(p_aRange, p_nSlots, 
            aTemp, nLenTmp );
         
     Delete(aTemp) ;
}
    
// --------------------
void sort(long* p_aRange, size_t p_nLenght, 
          long* p_aTemp,  size_t p_nLenTmp ) {
    buildRun( p_aRange, p_nLenght, 
              p_nLenght, 
              p_aTemp, p_nLenTmp );
}
   
/// Build run containing minLength elements
size_t buildRun(long*  p_aRange, size_t p_nLenght,
                size_t p_nMinLength, 
                long* p_aTemp,   size_t p_LenTmp 
               )  {
    if( p_nLenght <= MIN_RUN )   {
        binaryInsertionSort(p_aRange, p_nLenght, 1);
        return p_nLenght;
    }
    
    if( p_nMinLength >= p_nLenght / 2 ) 
        p_nMinLength = p_nLenght;
    
    // Length of current run
    size_t nCurr = firstRun(p_aRange, p_nLenght );
    
    if(nCurr < MIN_RUN)  {
        binaryInsertionSort( p_aRange, MIN_RUN, nCurr );
        nCurr = MIN_RUN;
    }
    
    while( nCurr < p_nMinLength ) {
        // size_t next = nCurr + buildRun( p_aRange[nCurr .. p_aRange.length], nCurr, p_aTemp);
        size_t next = nCurr + buildRun( p_aRange+nCurr, p_nLenght-nCurr, 
                                        nCurr, 
                                        p_aTemp, p_nLenTmp );
        merge(p_aRange,  next, 
              nCurr, 
              p_aTemp, p_nLenTmp );
        nCurr = next;
    }
    
    return nCurr;
}
 
void reverse(long* p_aRange, size_t p_nLeft, size_t p_nRight) {
    size_t nMidTop = p_nLeft + (p_nRight-p_nLeft)/2 + 1;
    long tmp ;
    for( size_t i=0; i < nMidTop ; ++i ) {
        tmp                  = p_aRange[i];
        p_aRange[i]          = p_aRange[p_nRight-i] ;
        p_aRange[p_nRight-i] = tmp ;
    }
}

/// Return length of first run in p_aRange
size_t firstRun(long*  p_aRange, size_t p_nLenght)  {
    assert(p_nLenght >= 2);
    
    size_t i = 2;
    if( p_aRange[0] <= p_aRange[1] ) {
        while( i < p_nLenght &&  p_aRange[i-1] < p_aRange[i] ) 
            ++i;
    }
    else  {
        while( i < p_nLenght && p_aRange[i-1] > p_aRange[i] ) 
            ++i;
        reverse( p_aRange, 0, i );
    }
    return i;
}

/// Merge two runs ***
void merge( long* p_aRange, size_t p_nLenght, 
            size_t p_nMid, 
            long*  p_aTemp, size_t p_nLenTmp 
          )  {
    while(true)      {
        assert(p_nMid <= p_nLenght);
        
        if( p_nMid == 0 || p_nMid == p_nLenght ) 
            return;
        
        if(gbInPlace)  {
            if( p_nLenght <= IN_PLACE && p_nMid * ( p_nLenght - p_nMid ) <= IN_PLACE) {
                mergeInsertion(p_aRange, p_nLenght, p_nMid);
                return;
            }
        }
        else {
            if(p_nMid <= p_nLenTmp) {
                mergeSmall(p_aRange, p_nLenght, p_nMid, 
                           p_aTemp, &p_nLenTmp);
                return;
            }
            else if(p_nLenght - p_nMid <= p_nLenTmp) {
                mergeSmallReverse(p_aRange, p_nLenght,
                                  p_nMid, 
                                  p_aTemp, &p_nLenTmp
                                 );
                return;
            }
        }
        
        long   *pLeft = p_aRange[0 .. p_nMid]
            , *pRight = p_aRange[p_nMid .. p_nLenght];
        size_t nLenLef = p_nMid + 1,
               nLenRig = p_nLenght - p_nMid + 1;
        
        if( p_aRange[p_nMid-1] < p_aRange[p_nMid] ) 
            return;
            
        size_t nSplit = mergeBig( pLeft, nLenLef,
                                 pRight, nLenRig );
        
        if(pLeft.length <= pRight.length) {
            merge( pLeft, nLenLef,
                   nSplit, 
                   p_aTemp );
            p_aRange = pRight;
            p_nMid = pLeft.length - nSplit;
        }
        else {
            merge(pRight, nLenRig,
                  nLenRig - nSplit, 
                  p_aTemp );
            p_aRange = pLeft;
            p_nMid   = nSplit;
        }
    }
}
    
    /// Merge two small runs from front to back
void mergeSmall(long* p_aRange, size_t p_nLenght,
                size_t p_nMid, 
                long* p_aTemp, size_t* p_pnLenTmp,
               ) {
    assert(p_nMid <= p_nLenght);
    
    *p_pnLenTmp = p_nMid + 1;
    size_t nLenTmp = *p_pnLenTmp ;
    memcpy(p_aTemp, p_aRange, sizeof(long)*(nLenTmp));
    
    size_t i        = 0, 
           nLeftTmp = 0, 
           nRight   = p_nMid;
    
    while(true) {
        if( p_aTemp[nLeftTmp] <= p_aRange[nRight]) )  {
            p_aRange[i++] = p_aTemp[nLeftTmp++];
            if(nLeftTmp >= nLenTmp) return;
        }
        else {
            p_aRange[i++] = p_aRange[nRight++];
            if(nRight >= p_nLenght) 
                while(true) {
                    p_aRange[i++] = p_aTemp[nLeftTmp++];
                    if(nLeftTmp >= nLenTmp) return;
                }
        }
    }
}
    
    /// Merge two small runs from back to front
void mergeSmallReverse(long* p_aRange, size_t p_nLenght,
                       size_t nMid, 
                       long* p_aTemp,  size_t* p_pnLenTmp
                      )    {
    assert(nMid <= p_aRange.length);

    *p_pnLenTmp = p_nLenght - nMid ;
    size_t nLenTmp = *p_pnLenTmp ;
    memcpy( p_aTemp, p_aRange+nMid, sizeof(long)*(p_pnLenTmp) ) ;
    
    size_t i         = p_nLenght - 1, 
           nLeft     = nMid - 1, 
           nRightTmp = nLenTmp - 1;
    
    while(true)  {
        if( p_aTemp[nRightTmp] >= p_aRange[nLeft] )  {
            p_aRange[i--] = p_aTemp[nRightTmp];
            if( nRightTmp == 0 ) return;
            --nRightTmp;
        } else {
            p_aRange[i--] = p_aRange[nLeft];
            if(nLeft == 0) 
                while(true) {
                    p_aRange[i--] = p_aTemp[nRightTmp];
                    if(nRightTmp == 0) return;
                    --nRightTmp;
                }
            --nLeft;
        }
    }
}
    
/// Use insertion to merge two runs in-place
void mergeInsertion(long* p_aRange, size_t p_nLenght,  size_t p_nMid)  {
    size_t nLeft = 0, 
           nRight = p_nMid, 
           i;
    long datum;
    
    while(true) {
        if( p_aRange[nLeft] <= p_aRange[nRight] ) {
            ++nLeft;
            if(nLeft >= nRight) 
                break;
        }
        else {
            datum = p_aRange[nRight];
            for(i = nRight; i > nLeft; --i) 
                p_aRange[i] = p_aRange[i-1];
            p_aRange[i] = datum;
            ++nLeft; 
            ++nRight;
            if(nRight >= p_nLenght ) 
                break;
        }
    }
}
    
/// Reduce two large runs into four smaller runs
size_t mergeBig(  long* pLeft,  size_t p_nLenLeft
                , long* pRight, size_t p_nLenRight ) {
    
    size_t nLower
    if( p_nLenLeft <= p_nLenRight )
        nLower = 0 ;
    else 
        nLower = p_nLenLeft - p_nLenRight ;
        
    size_t  nMiddle
          , nUpper = p_nLenLeft - 1 
          , nOff   = nUpper ;
          
    while( nUpper != nLower ) {
        // This expression is written as to avoid integer overflow
        nMiddle = nLower + (nUpper - nLower) / 2 ;
        if( pLeft[nMiddle] > pRight[nOff - nMiddle] ) 
            nUpper = nMiddle;
        else 
            nLower = nMiddle + 1;
    }
    
    swapBlocks(  pLeft+nLower,  p_nLenLeft - nLower + 1
               , pRight+0, nOff - nLower + 1 ) ;
    
    return nLower;
}
    
/// Swap two (adjacent) ranges of elements
void swapBlocks(  long* pLeft,  size_t p_nLenLeft
                , long* pRight, size_t p_nLenRight
               ) {
    if( p_nLenLeft == p_nLenRight )
        return ;
    long datum;
    for(size_t i=0 ; i < p_nLenLeft ; ++i ) {
        datum     = pLeft[i];
        pLeft[i]  = pRight[i];
        pRight[i] = datum ;
    }
}
    
    
//@ Workaround for DMD issue 7898
static if( __VERSION__ == 2059)
void copy(R1, R2)(R1 src, R2 dst)  {
    import std.traits;
    static if(isArray!R1 && isArray!R2) 
        if(__ctfe)   {
            dst[] = src[];
            return;
        }
    std.algorithm.copy(src, dst);
}
    


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// bool testSort(alias pred, bool gbInPlace = false, R)(R p_aRange) {
    // stableSort!(pred, gbInPlace, R)(p_aRange);
    // return isSorted!pred(p_aRange);
// }

int testCall(T)(in T[] arr) {
    int failures = 0;
    /*
    // Sort
    if(!testSort!"a < b"(arr.dup)) ++failures;
    if(!testSort!"a > b"(arr.dup)) ++failures;
    
    // In-place sort
    if(!testSort!("a < b", true)(arr.dup)) ++failures;
    if(!testSort!("a > b", true)(arr.dup)) ++failures;
    /**/
    
    return failures;
}

bool icmp(unsigned char a, unsigned char b)    {
    if(a >= 'a') a -= 'a' - 'A';
    if(b >= 'a') b -= 'a' - 'A';
    return a < b;
}

void main() {
    
    // Array containing 256 random ints
    enum test = [
        10, 37, 74, 99, 86, 28, 17, 39, 18, 38, 70, 89, 94, 32, 46, 76, 43, 33, 62, 76, 
        37, 93, 45, 48, 49, 21, 67, 56, 58, 17, 15, 41, 91, 94, 95, 41, 38, 80, 37, 24, 
        26, 71, 87, 54, 72, 60, 29, 37, 41, 99, 31, 66, 75, 72, 86, 97, 37, 25, 98, 89, 
        53, 45, 52, 76, 51, 38, 59, 53, 74, 96, 94, 42, 68, 84, 65, 27, 49, 57, 53, 74, 
        39, 75, 39, 26, 46, 37, 68, 96, 19, 79, 73, 83, 36, 90, 11, 39, 48, 94, 97, 72, 
        37, 43, 69, 36, 41, 47, 31, 48, 33, 21, 20, 18, 45, 28, 47, 54, 41, 28, 47, 44, 
        51, 15, 21, 64, 82, 23, 41, 82, 30, 25, 78, 72, 50, 34, 45, 59, 14, 71, 50, 97, 
        39, 87, 74, 60, 52, 17, 87, 45, 69, 54, 91, 68, 46, 99, 78, 33, 27, 53, 41, 84, 
        82, 54, 29, 55, 53, 87, 13, 98, 55, 33, 73, 64, 19, 81, 57, 78, 23, 45, 94, 75, 
        55, 43, 93, 85, 96, 82, 44, 73, 22, 79, 89, 20, 36, 11, 12, 51, 86, 86, 75, 66, 
        81, 90, 80, 80, 36, 36, 47, 43, 86, 96, 45, 73, 70, 90, 57, 23, 86, 29, 12, 54, 
        37, 17, 87, 12, 36, 78, 26, 28, 30, 15, 10, 53, 76, 34, 23, 49, 65, 17, 37, 51, 
        26, 23, 66, 12, 26, 84, 60, 47, 30, 26, 78, 20, 42, 40, 63, 40
    ] ;
    
    // Runtime test
    assert(testCall(test) == 0);
    
    // CTFE Test
    {
        enum result = testCall(test);
        static if(result != 0) pragma(msg, __FILE__, "(", __LINE__, "): Warning: stableSort CTFE unittest failed ", result, " of 4 tests");
    }
    
    // Stability test
    ubyte[] str = cast(ubyte[])"ksugnqtoyedwpvbmifaclrhjzxWELPGDVJIHBAMZCFUNORKSTYXQ".dup;
    stableSort!icmp(str);
    assert(str == "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");
//----------------------------
    
}