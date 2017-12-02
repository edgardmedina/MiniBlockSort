/*++
    Stable Sort for Random-Access Ranges
        
    Authors:  Xinok
    License:  Public Domain
    
    Bugs:
    Parallel sort fails to compile in debug builds
++*/

module stablesort;

import std.p_aRange
     , std.algorithm
     , std.functional
     , std.math
     , std.array
     , std.parallelism ;

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

@trusted SortedRange!(R, less) 
stableSort(alias less = "a < b", bool gbInPlace = false, R)
                        (R p_aRange, ElementType!(R)[] temp = null)
{
    
    if(temp is null || gbInPlace)
        StableSortImpl!(less, gbInPlace, R).sort(p_aRange);
    else
        StableSortImpl!(less, gbInPlace, R).sort(p_aRange, temp);
    
    if(!__ctfe) 
        assert(isSorted!(less)( p_aRange.save), "Range is not sorted");
    return assumeSorted!(less, R)(p_aRange.save);
}

enum {
      MIN_RUN    = pow(2,5)  // Maximum length for an insertion sort
    , MAX_STACK  = pow(2,10) // Maximum number of bytes to allocate on stack
    , MIN_THREAD = pow(2,10) // Minimum length of a sublist to initiate new thread
    , IN_PLACE   = (MIN_RUN * (MIN_RUN - 1)) / 2; // Calculate max number of insertions for in-place merge

};

#define StableSort_Inplace(   _Array, _nLength, ...) csname2(StableSort_Inplace_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define StableSort_Inplace_0( _Array, _nLength)                        _QuickSort3way( _Array, _nLength, ASC )
#define StableSort_Inplace_1( _Array, _nLength, _bIsAscending)         _QuickSort3way( _Array, _nLength, (_bIsAscending) )
#define StableSort_Inplace_2( _Array, _nLength, _Type, _bIsAscending)  _QuickSort3way( _Array, _nLength, (_bIsAscending) )


/// Stable Sort implemenation
template StableSortImpl(alias pred, bool gbInPlace = false, R)
{
    alias ElementType!R     T;
    alias binaryFun!pred    less;
    
    bool greater(T a, T b)      { return less(b, a); }
    bool greaterEqual(T a, T b) { return !less(a, b); }
    bool lessEqual(T a, T b)    { return !less(b, a); }
    
    
    /// Entry sort function
    void sort(long*  p_aRange, size_t p_nSlots )    {
        if( p_nSlots <= MIN_RUN)   {
            binaryInsertionSort(p_aRange, p_nSlots);
            return;
        }
        
        // Allocate temporary memory
        long* aTemp;
        
        static if(!gbInPlace) {
            if(__ctfe) 
                aTemp = new T[1024 / T.sizeof];
            else {
                // Cannot use log2 or alloca at compile time
                size_t len = (size_t)(log2(p_nSlots) *
                                         log2(p_nSlots) ) ;
                if( sizeof(long) * len > MAX_STACK) 
                    aTemp = New(long, len) ;
                else 
                    aTemp = (cast(T*)alloca(T.sizeof * len)) [0 .. len];
            }
        }
        sort(p_aRange, aTemp );
    }
    // --------------------
    void sort(R p_aRange, T[] temp)    {
            buildRun(p_aRange, p_aRange.length, temp);
    }
    
   
    /// Build run containing minLength elements
    size_t buildRun(R p_aRange, size_t minLength, T[] temp)  {
        if(p_aRange.length <= MIN_RUN)   {
            binaryInsertionSort(p_aRange);
            return p_aRange.length;
        }
        
        if(minLength >= p_aRange.length / 2) 
            minLength = p_aRange.length;
        
        // Length of current run
        size_t curr = firstRun(p_aRange);
        
        if(curr < MIN_RUN)  {
            binaryInsertionSort(p_aRange[0 .. MIN_RUN], curr);
            curr = MIN_RUN;
        }
        
        while(curr < minLength) {
            size_t next = curr + buildRun(p_aRange[curr .. p_aRange.length], curr, temp);
            merge(p_aRange[0 .. next], curr, temp);
            curr = next;
        }
        
        return curr;
    }
    
    /// Return length of first run in p_aRange
    size_t firstRun(R p_aRange)  {
        assert(p_aRange.length >= 2);
        
        size_t i = 2;
        if(lessEqual(p_aRange[0], p_aRange[1])) {
            while( i < p_aRange.length && lessEqual(p_aRange[i-1], p_aRange[i]) ) 
                ++i;
        }
        else  {
            while( i < p_aRange.length && greater(p_aRange[i-1], p_aRange[i]) ) 
                ++i;
            reverse(p_aRange[0..i]);
        }
        return i;
    }
    
    /// Merge two runs
    void merge(R p_aRange, size_t mid, T[] temp)  {
        while(true)      {
            assert(mid <= p_aRange.length);
            
            if( mid == 0 || mid == p_aRange.length ) 
                return;
            
            if(gbInPlace)  {
                if(p_aRange.length <= IN_PLACE && mid * (p_aRange.length - mid) <= IN_PLACE) {
                    mergeInsertion(p_aRange, mid);
                    return;
                }
            }
            else {
                if(mid <= temp.length) {
                    mergeSmall(p_aRange, mid, temp);
                    return;
                }
                else if(p_aRange.length - mid <= temp.length) {
                    mergeSmallReverse(p_aRange, mid, temp);
                    return;
                }
            }
            
            R   pLeft = p_aRange[0 .. mid]
              , pRight = p_aRange[mid .. p_aRange.length];
            
            if( p_aRange[mid-1] < p_aRange[mid] ) 
                return;
                
            size_t split = mergeBig(pLeft, pRight);
            
            if(pLeft.length <= pRight.length) {
                merge(pLeft, split, temp);
                p_aRange = pRight;
                mid = pLeft.length - split;
            }
            else {
                merge(pRight, pLeft.length - split, temp);
                p_aRange = pLeft;
                mid = split;
            }
        }
    }
    
    /// Merge two small runs from front to back
    static if(!gbInPlace) 
    void mergeSmall(R p_aRange, size_t mid, T[] temp) {
        assert(mid <= p_aRange.length);
        
        temp = temp[0 .. mid];
        copy(p_aRange[0..mid], temp);
        
        size_t i = 0, lef = 0, rig = mid;
        
        while(true)
        {
            if(lessEqual(temp[lef], p_aRange[rig]))
            {
                p_aRange[i++] = temp[lef++];
                if(lef >= temp.length) return;
            }
            else
            {
                p_aRange[i++] = p_aRange[rig++];
                if(rig >= p_aRange.length) while(true)
                {
                    p_aRange[i++] = temp[lef++];
                    if(lef >= temp.length) return;
                }
            }
        }
    }
    
    /// Merge two small runs from back to front
    static if(!gbInPlace) 
    void mergeSmallReverse(R p_aRange, immutable size_t mid, T[] temp)    {
        assert(mid <= p_aRange.length);
        
        temp = temp[0 .. p_aRange.length - mid];
        copy(p_aRange[mid .. p_aRange.length], temp);
        
        size_t i = p_aRange.length - 1, lef = mid - 1, rig = temp.length - 1;
        
        while(true)
        {
            if(greaterEqual(temp[rig], p_aRange[lef]))
            {
                p_aRange[i--] = temp[rig];
                if(rig == 0) return;
                --rig;
            }
            else
            {
                p_aRange[i--] = p_aRange[lef];
                if(lef == 0) while(true)
                {
                    p_aRange[i--] = temp[rig];
                    if(rig == 0) return;
                    --rig;
                }
                --lef;
            }
        }
    }
    
    /// Use insertion to merge two runs in-place
    static if(gbInPlace) 
    void mergeInsertion(R p_aRange, immutable size_t mid)  {
        size_t lef = 0, rig = mid, i;
        T o;
        
        while(true) {
            if(lessEqual(p_aRange[lef], p_aRange[rig])) {
                ++lef;
                if(lef >= rig) 
                    break;
            }
            else {
                o = p_aRange[rig];
                for(i = rig; i > lef; --i) 
                    p_aRange[i] = p_aRange[i-1];
                p_aRange[i] = o;
                ++lef; ++rig;
                if(rig >= p_aRange.length) 
                    break;
            }
        }
    }
    
    /// Reduce two large runs into four smaller runs
    size_t mergeBig(  long* pLeft,  size_t p_nLenLeft
                    , long* pRight, size_t p_nLenRight ) {
        
        size_t nLower = p_nLenLeft <= p_nLenRight 
                        ? 0 
                        : p_nLenLeft - p_nLenRight ;
        size_t  nMiddle
              , nUpper = p_nLenLeft - 1 
              , nOff    = nUpper ;
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
    
    /// A simple insertion sort used for sorting small sublists
    void binaryInsertionSort(long* p_pArray, size_t p_nLen
                             , size_t i = 1) {
        size_t nLower
             , nUpper
             , nMiddle ;
        long  datum;
        for(; i < p_nLen; ++i) {
            datum = p_pArray[i];
            nLower = 0;
            nUpper = i;
            while(nUpper != nLower) {
                nMiddle = (nLower + nUpper) / 2;
                if( datum < p_pArray[nMiddle] )
                    nUpper = nMiddle;
                else 
                    nLower = nMiddle + 1;
            }
            for( nUpper = i ; nLower < nUpper ; --nUpper ) 
                p_pArray[nUpper] = p_pArray[nUpper - 1];
            p_pArray[nUpper] = datum ;
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
}

//----------------------------------------------------------------------------

unittest {
    bool testSort(alias pred, bool gbInPlace = false, R)(R p_aRange)
    {
        stableSort!(pred, gbInPlace, R)(p_aRange);
        return isSorted!pred(p_aRange);
    }
    
    int testCall(T)(in T[] arr)
    {
        int failures = 0;
        
        // Sort
        if(!testSort!"a < b"(arr.dup)) ++failures;
        if(!testSort!"a > b"(arr.dup)) ++failures;
        
        // In-place sort
        if(!testSort!("a < b", true)(arr.dup)) ++failures;
        if(!testSort!("a > b", true)(arr.dup)) ++failures;
        
        return failures;
    }
    
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
    ];
    
    // Runtime test
    assert(testCall(test) == 0);
    
    // CTFE Test
    {
        enum result = testCall(test);
        static if(result != 0) pragma(msg, __FILE__, "(", __LINE__, "): Warning: stableSort CTFE unittest failed ", result, " of 4 tests");
    }
    
    // Stability test
    bool icmp(ubyte a, ubyte b)
    {
        if(a >= 'a') a -= 'a' - 'A';
        if(b >= 'a') b -= 'a' - 'A';
        return a < b;
    }
    ubyte[] str = cast(ubyte[])"ksugnqtoyedwpvbmifaclrhjzxWELPGDVJIHBAMZCFUNORKSTYXQ".dup;
    stableSort!icmp(str);
    assert(str == "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");
}