/*****************************************************************************************************
/***************************************************************************************
 * @Algorithm :  Merge Sort.
 *               Generic adaptation in 'C' pure, to works in 'C' and/or C++ from:
 *                 @last modified: 09 Jan 1998
 * 
 * @Adapted by: Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, Perú.
 * 
 * @Observation: This algorith is generic and minimun requiere ISO C99 or C++98,
 *               suport native types or custom C/C++ Structures (variables) or C++ Classes (objects).
 * 
 *               To work correctly is necessary define a Funtor compare with prototype:
 *                   int (*fnCompare)(const void*, const void*);
 * 
 *               NOTE: in C++, the funtor must be declared as 'friend function' to Class.
 * 
 *****************************************************************************************************
 *
 * MergeSort(): MergeSort algorithm when specified only the basic <Type>.
 *              Previously must declare the functor name with a format:
 * 
 *                   int <Type>_Compare(const void*, const void*);
 * 
 * Syntax:  void MergeSort( 
 *                           <Type>* pArray        // Valid Array to sort.
 *                          ,size_t  nLength       // Length of array.
 *                          ,(<Type>)              // type of data in array.
 *                          , bool   IsAscending   // Boolean Flag
 *                                                 //   - true : the sortering is ascending.
 *                                                 //   - false: the sortering is descending.
 *                         )                       // Returns: the array sorted.
 * 
 *---------------------------------------------------------------------------------------------
 *
 * MergeSort_fn(): MergeSort algorithm when specified a funtor compare.
 *                 the funtor can has any name.
 *                   int <Type>fnCompare(const void*, const void*);
 * 
 * Syntax:  void MergeSort_fn( 
 *                           <Type>*     pArray        // Valid Array to sort.
 *                          ,size_t      p_nLength     // Length of array.
 *                          ,fnCompare_t fnCompare     // Funtor Compare
 *                          , bool       p_IsAscending // Boolean Flag
 *                                                     //   - true : the sortering is ascending.
 *                                                     //   - false: the sortering is descending.
 *                         )                           // Returns: the array sorted.
 * 
 *****************************************************************************************************/
#define C_TEMPLATE
#include "ebm_base.h"
 
static void Merge_Asc( void* pFirst, void* pMidle, void* pLast, size_t nBytes, fnCompare_t fnCompare, void* pArray, void* Buffer ) ;
static void Merge_Desc(void* pFirst, void* pMidle, void* pLast, size_t nBytes, fnCompare_t fnCompare, void* pArray, void* Buffer ) ;


inline void  _MergeSort_fn( void* pArray, size_t nSlots, size_t nBytesSize, fnCompare_t fnCompare, bool bIsAscending ) {
   if (nLength < 2) return;
   void* Buffer[nLength*nBytesSize];
   if( p_bAscending ) 
      MergeSort_Asc( pArray, pArray+(nLength-1)*nBytesSize, nBytesSize, pArray, Buffer );
   else
      MergeSort_Desc(pArray, pArray+(nLength-1)*nBytesSize, nBytesSize, pArray, Buffer );
}

#define MergeSort(    _pArray, _nLength, _Type, ... )       csname2(MergeSort_fn_, Count_Args(__VA_ARGS__))( (_pArray), (_nLength), FNCOMPARE(_Type), ##__VA_ARGS__)
#define MergeSort_fn( _pArray, _nLength, _fnCompare, ... )  csname2(MergeSort_fn_, Count_Args(__VA_ARGS__))( (_pArray), (_nLength), _fnCompare, ##__VA_ARGS__)

#define MergeSort_fn_0( _pArray, _nLength, _fnCompare )                 _MergeSort_fn( (void*)_pArray, _nLength, sizeof(*_pArray), _fnCompare, ASC ) 
#define MergeSort_fn_1( _pArray, _nLength, _fnCompare, _bIsAscending )  _MergeSort_fn( (void*)_pArray, _nLength, sizeof(*_pArray), _fnCompare, _bIsAscending ) 

// ...............................................

static void MergeSort_Asc(void*  pFirst, void* pLast, size_t nBytes, fnCompare_t fnCompare, void* pArray, void* Buffer ) {
   if (nFirst < nLast) {
      int   factor = nBytes * (((pLast -pFirst)/nBytes+1)%2 == 0 ? 1 : 0) ;
      void* pMidle = pFirst + ((size_t)pLast - (size_t)pFirst - factor ) / 2 ;

      // Sort first and second halves
      MergeSort_Asc(  pFirst,        pMidle, nBytes, fnCompare, pArray, Buffer );
      MergeSort_Asc(  pMidle+nBytes, pLast,  nBytes, fnCompare, pArray, Buffer );

      Merge_Asc( pFirst, pMidle, pLast, nBytes, fnCompare, pArray, Buffer);
   }
}

// ...............................................

static void Merge_Asc(void* pFirst, void* pMidle, void* pLast, size_t nBytes, fnCompare_t fnCompare, void* pArray, void* Buffer ) {
    int i, j, k;
    int nOffset = pFirst - pArray ;
    int nBuffer1 = pMidle - pFirst + nBytes;
    int nBuffer2 = pLast - pMidle;

    /* Reference to Temporal Buffers */
    int *L = Buffer + nOffset, 
        *R = Buffer + nOffset + nBuffer1,
        *LStop = R,
        *RStop = R + nBuffer2 ;

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < nBuffer1; ++i )  L[i] = arr[nFirst + i];
    for (j = 0; j < nBuffer2; ++j )  R[j] = arr[nMidle + 1+ j];

    /* Merge the temp arrays back into arr[nFirst..nLast]*/
    i = 0;        // Initial index of first subarray
    j = 0;        // Initial index of second subarray
    k = nFirst;   // Initial index of merged subarray
    while (i < nBuffer1 && j < nBuffer2) {
        if ( L[i] <= R[j] ) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[] and R[].
    while (i < nBuffer1)  arr[k++] = L[i++]; // From L[], if there are any
    while (j < nBuffer2)  arr[k++] = R[j++]; // From R[], if there are any
}

// ...............................................

static void MergeSort_Desc(long arr[], int nFirst, int nLast, long ans[]) {
    if (nFirst < nLast) {
      int   factor = nBytes * (((pLast -pFirst)/nBytes+1)%2 == 0 ? 1 : 0) ;
      void* pMidle = pFirst + ((size_t)pLast - (size_t)pFirst - factor ) / 2 ;

        // Sort first and second halves
        MergeSort_Desc(arr, nFirst,   nMidle, ans );
        MergeSort_Desc(arr, nMidle+1, nLast, ans );

        Merge_Desc(arr, nFirst, nMidle, nLast, ans);
    }
}

// .......................................................

static void Merge_Desc(long arr[], int nFirst, int nMidle, int nLast, long Buffer[] ) {
    int i, j, k;
    int nBuffer1 = nMidle - nFirst + 1;
    int nBuffer2 =  nLast - nMidle;

    
    // int L[nBuffer1], R[nBuffer2];      // create temp arrays. 
    /* Reference to Temporal Buffers */
    int *L=Buffer+nFirst, 
        *R=Buffer+nFirst+nBuffer1;

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < nBuffer1; ++i )  L[i] = arr[nFirst + i];
    for (j = 0; j < nBuffer2; ++j )  R[j] = arr[nMidle + 1+ j];

    /* Merge the temp arrays back into arr[nFirst..nLast]*/
    i = 0;        // Initial index of first subarray
    j = 0;        // Initial index of second subarray
    k = nFirst;   // Initial index of merged subarray
    while (i < nBuffer1 && j < nBuffer2) {
        if ( L[i] >= R[j] ) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[] and R[].
    while (i < nBuffer1)  arr[k++] = L[i++]; // From L[], if there are any
    while (j < nBuffer2)  arr[k++] = R[j++]; // From R[], if there are any
}
//-------------------------------------------------------------------------
