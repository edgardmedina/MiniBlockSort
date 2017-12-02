#ifndef  __mergesort_h__
#define  __mergesort_h__

#include "binary_insertsort.h"

#define MergeSort( _Array, ...)     csname2(MergeSort_, Count_Args(__VA_ARGS__))( (_Array), ##__VA_ARGS__)
#define MergeSort_0( _Array )                                MERGE_SORT( _Array, sizeofarray(_Array))
#define MergeSort_1( _Array, _nSlots)                        MERGE_SORT( _Array, (_nSlots) )
#define MergeSort_2( _Array, _nSlots, _bIsAscending )        MERGE_SORT( _Array, (_nSlots) )
#define MergeSort_3( _Array, _nSlots, _Type, _bIsAscending)  MERGE_SORT( _Array, (_nSlots) )

/* Standard merge sort */
void MERGE_SORT(long *p_pArray, const size_t size) {
  const size_t middle = size / 2;
  size_t out = 0,
           i = 0,
           j = middle;

  /* don't bother sorting an array of size <= 1 */
  if (size <= 1) {
    return;
  }

  if (size < 16) {
    BINARY_INSERTION_SORT(p_pArray, size);
    return;
  }

  MERGE_SORT(p_pArray, middle);
  MERGE_SORT(&p_pArray[middle], size - middle);
  
  long *pBuff = (long *) malloc(size * sizeof(long));

    while (out != size) {
        if (i < middle) {
            if (j < size) {
                if (SORT_CMP(p_pArray[i], p_pArray[j]) <= 0) {
                    pBuff[out] = p_pArray[i++];
                } else {
                    pBuff[out] = p_pArray[j++];
                }
            } else {
                pBuff[out] = p_pArray[i++];
            }
        } else {
            pBuff[out] = p_pArray[j++];
        }

        out++;
    }

  memcpy(p_pArray, pBuff, size * sizeof(long));
  free(pBuff);
}


#endif  // #ifdef  __mergesort2_h__
