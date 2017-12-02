/***************************************************************************************
 * @Algorithm :  Optimized Insert Sort and Binary Insert Sort.
 *               Simple adaptation to 'C99' or 'C++98' from:
 *                 Optimized insert sort and Binary Insert Sort from 
 *                 @author       : Jeffrey Stedfast
 *                 @version      : 25-02-2007
 *                 @from         : https://jeffreystedfast.blogspot.pe/2007/02/insertion-sort.html
 *                                 https://jeffreystedfast.blogspot.pe/2007/02/binary-insertion-sort.html
 *
 * @Adapted by: Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, Perú.
 ***************************************************************************************/
#ifndef  __binary_insert_sort_h__
#define  __binary_insert_sort_h__

#define BinaryInsertionSort2( _Array, nLength, ...)      csname2(BinaryInsertionSort2_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define BinaryInsertionSort2_0( Array, nLength)                        __BinaryInsertionSort( (Array), (nLength) )
#define BinaryInsertionSort2_1( Array, nLength, _bIsAscending )        __BinaryInsertionSort( (Array), (nLength) )
#define BinaryInsertionSort2_2( Array, nLength, _Type, _bIsAscending)  __BinaryInsertionSort( (Array), (nLength) )

void __BinaryInsertionSort(long p_pArray[], int p_nLen) {
    register int nIter, nMiddle,
                 nHigh, nLow;
    long Datum , tmp ;

    for( nIter = 1; nIter < p_nLen; ++nIter ) {
        Datum = p_pArray[nIter] ;
        //--------------[Binary Search]--------------\\-
        nLow    = 0, 
        nHigh   = nIter ;
        // nMiddle = nIter / 2 ;
        nMiddle = Middle(nIter) ;
        do {
            if( Datum > p_pArray[nMiddle] ) 
                nLow = nMiddle + 1;
            else if( Datum < p_pArray[nMiddle] ) 
                nHigh = nMiddle;
            else
                break;
            // nMiddle = nLow + ((nHigh - nLow) / 2);
            nMiddle = Middle(nLow,  nHigh) ;
        } while( nLow < nHigh );    // search a 'nLow' position
        //\\-----------------------------------------//-
        if( nMiddle < nIter ) {
            // tmp = Datum ;
            memmove(  p_pArray + nMiddle + 1
                    , p_pArray + nMiddle
                    , sizeof(long) * (nIter - nMiddle)
                   );
            // p_pArray[nMiddle] = tmp;
            p_pArray[nMiddle] = Datum;
        }
    }
}
// -------------------------------------------------
int BinarySearch2(int a[], int low, int high, int key) {
    int mid;

    if (low == high)
        return low;

    // mid = low + ((high - low) / 2);
    mid = Middle(low, high) ;

    if(  a[mid] < key )
        return BinarySearch2(a, mid + 1, high, key);
    else if (key < a[mid])
        return BinarySearch2(a, low, mid, key);

    return mid;
}

#define BinInsSort( _Array, nLength, ...)      csname2(BinInsSort_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define BinInsSort_0( Array, nLength)                        __BinaryInsertionSort2( (Array), (nLength) )
#define BinInsSort_1( Array, nLength, _bIsAscending )        __BinaryInsertionSort2( (Array), (nLength) )
#define BinInsSort_2( Array, nLength, _Type, _bIsAscending)  __BinaryInsertionSort2( (Array), (nLength) )

//  Classic BinaryInsertionSort()
void __BinaryInsertionSort2(int a[], int n) {
    int ins, i, j;
    int tmp;

    for (i = 1; i < n; i++) {
        ins = BinarySearch2(a, 0, i, a[i]);
        if (ins < i) {
            tmp = a[i];
            for (j = i - 1; j >= ins; j--)
                a[j + 1] = a[j];
            a[ins] = tmp;
        }
    }
}

#define BinInsSort2( _Array, nLength, ...)      csname2(BinInsSort2_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define BinInsSort2_0( Array, nLength)                        __BinaryInsertionSort3( (Array), (nLength) )
#define BinInsSort2_1( Array, nLength, _bIsAscending )        __BinaryInsertionSort3( (Array), (nLength) )
#define BinInsSort2_2( Array, nLength, _Type, _bIsAscending)  __BinaryInsertionSort3( (Array), (nLength) )

void __BinaryInsertionSort3 (int a[], int n) {
    int ins, i, j;
    int tmp;

    for (i = 1; i < n; i++) {
        ins = BinarySearch2 (a, 0, i, a[i]);
        tmp = a[i];
        for (j = i - 1; j >= ins; j--)
            a[j + 1] = a[j];
        a[ins] = tmp;
    }
}

// -------------------------------------------------

#endif  // #ifndef __binary_insert_sort2_h__

