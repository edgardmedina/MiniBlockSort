/***************************************************************************************
 * @Algorithm :  Quick Sort.
 *               Simple adaptation to 'C99' or 'C++98' from:
 *                 @Source       : DualPivotQuicksort.java
 *                 @author       : Vladimir Yaroslavskiy
 *                 @version      : 2009.09.17 m765.817
 * 
 * @Adapted by: Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, Perú.
 ***************************************************************************************/

//-------------------------------------------------------------------------
const int DIST_SIZE = 13;
const int TINY_SIZE = 17;
   
void DualPivotQuickSort_Asc(long Array[], int nFirst, int nLast);
void DualPivotQuickSort_Desc(long Array[], int nFirst, int nLast);

#define DualPivotQuickSort( _Array, nLength, ...) csname2(DualPivotQuickSort_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define DualPivotQuickSort_0( Array, nLength)                        _DualPivotQuickSort( (Array), (nLength), ASC )
#define DualPivotQuickSort_1( Array, nLength, _bIsAscending)         _DualPivotQuickSort( (Array), (nLength), (_bIsAscending) )
#define DualPivotQuickSort_2( Array, nLength, _Type, _bIsAscending)  _DualPivotQuickSort( (Array), (nLength), (_bIsAscending) )

inline static void _DualPivotQuickSort(long* Array, size_t nLength, bool p_bAscending ) {
   if( p_bAscending ) 
      DualPivotQuickSort_Asc(Array, 0, nLength-1);
   else
      DualPivotQuickSort_Desc(Array, 0, nLength-1);
}

// ...............................................
   
static void DualPivotQuickSort_Asc(long Array[], int nFirst, int nLast) {
   int nStop = nLast + 1;
   int nLen = nLast - nFirst;
   long vTemp;
   // Step 1. For small arrays (length < 17), use the Insertion sort algorithm.
   if (nLen < TINY_SIZE) { // insertion sort on tiny array
      for (int i = nFirst + 1; i < nStop; i++) {
         for (int j = i; nFirst < j  && Array[j] < Array[j - 1]; j--) 
            Swap( Array[j - 1], Array[j], vTemp );
      }
      return;
   }

   // median indexes
   int sixth = nLen / 6;
   int  nLeft = nFirst + sixth;
   int     nL = nLeft + sixth;
   int     nK = nL + sixth;
   int     nG = nK + sixth;
   int nRight = nG + sixth;

   // printf("\n");
   // printf(" Arra[%d]=%d,\t particion size= %d\n", nFirst, Array[nFirst], sixth );
   // printf(" nLeft[%d]=%d\n", nLeft, Array[nLeft] );
   // printf(" nL[%d]=%d\n", nL, Array[nL] );
   // printf(" nK[%d]=%d\n", nK, Array[nK] );
   // printf(" nG[%d]=%d\n", nG, Array[nG] );
   // printf(" nRight[%d]=%d\n", nRight, Array[nRight] );
   // printf(" Arra[%d]=%d\n", nLast, Array[nLast] );
   
   // 5-element sorting network
   if ( Array[nL]     < Array[nLeft] ) Swap( Array[nL],     Array[nLeft], vTemp );
   if ( Array[nRight] < Array[nG]    ) Swap( Array[nRight], Array[nG],    vTemp );
   if ( Array[nK]     < Array[nLeft] ) Swap( Array[nK],     Array[nLeft], vTemp );
   if ( Array[nK]     < Array[nL]    ) Swap( Array[nK],     Array[nL],    vTemp );
   if ( Array[nG]     < Array[nLeft] ) Swap( Array[nG],     Array[nLeft], vTemp );
   if ( Array[nG]     < Array[nK]    ) Swap( Array[nG],     Array[nK],    vTemp );
   if ( Array[nRight] < Array[nL]    ) Swap( Array[nRight], Array[nL],    vTemp );
   if ( Array[nK]     < Array[nL]    ) Swap( Array[nK],     Array[nL],    vTemp );
   if ( Array[nRight] < Array[nG]    ) Swap( Array[nRight], Array[nG],    vTemp );
   // printf("\n");
   // printf(" Arra[%d]=%d\n", nFirst, Array[nFirst] );
   // printf(" nLeft[%d]=%d\n", nLeft, Array[nLeft] );
   // printf(" nL[%d]=%d\n", nL, Array[nL] );
   // printf(" nK[%d]=%d\n", nK, Array[nK] );
   // printf(" nG[%d]=%d\n", nG, Array[nG] );
   // printf(" nRight[%d]=%d\n", nRight, Array[nRight] );
   // printf(" Arra[%d]=%d\n", nLast, Array[nLast] );
   
   // pivots: [ < vPivot1 | vPivot1 <= && <= vPivot2 | > vPivot2 ]
   long vPivot1 = Array[nL];
   long vPivot2 = Array[nG];
   bool bDiffPivots = vPivot1 != vPivot2;
   
   Array[nL] = Array[nFirst];
   Array[nG] = Array[nLast];

   // printf("\n");
   // printf(" Arra[%d]=%d\n", nFirst, Array[nFirst] );
   // printf(" nLeft[%d]=%d\n", nLeft, Array[nLeft] );
   // printf(" nL[%d]=%d\n", nL, Array[nL] );
   // printf(" nK[%d]=%d\n", nK, Array[nK] );
   // printf(" nG[%d]=%d\n", nG, Array[nG] );
   // printf(" nRight[%d]=%d\n", nRight, Array[nRight] );
   // printf(" Arra[%d]=%d\n", nLast, Array[nLast] );
   // printf(" Pivot 1=%d\n", vPivot1 );
   // printf(" Pivot 2=%d\n", vPivot2 );
   
   // center part pointers
   int nLow  = nFirst + 1;
   int nHigh = nLast - 1;

   // sorting
   if (bDiffPivots) {
      for (int i = nLow; i <= nHigh; i++) {
         if (Array[i] < vPivot1) {
            Swap( Array[i], Array[nLow], vTemp) ;
            ++nLow;
         } 
         else if ( Array[i] > vPivot2) {
            while (Array[nHigh] > vPivot2 && i < nHigh)  --nHigh;
            Swap( Array[i], Array[nHigh], vTemp );
            --nHigh;
            
            if ( Array[i] < vPivot1) {
               Swap( Array[i], Array[nLow], vTemp);
               ++nLow;
            }
         }
      }
   } else {
      for (int i = nLow; i <= nHigh; i++) {
         if ( Array[i] == vPivot1)  continue;
         if ( Array[i] < vPivot1) {
            Swap( Array[i], Array[nLow], vTemp);
            ++nLow;
         }
         else {
            while( Array[nHigh] > vPivot2 && i < nHigh ) --nHigh;
            Swap( Array[i], Array[nHigh], vTemp ) ;
            --nHigh;
            
            if (Array[i] < vPivot1) {
               Swap( Array[i], Array[nLow], vTemp) ;
               ++nLow ;
            }
         }
      }
   }

   // swap
   Array[nFirst] = Array[nLow - 1];
   Array[nLow - 1] = vPivot1;

   Array[nLast] = Array[nHigh + 1];
   Array[nHigh + 1] = vPivot2;

   // nFirst and nLast parts
   DualPivotQuickSort_Asc(Array, nFirst, nLow - 2);
   DualPivotQuickSort_Asc(Array, nHigh + 2, nLast);

   // equal elements
   if (nHigh - nLow > nLen - DIST_SIZE && bDiffPivots) {
      for (int k = nLow; k <= nHigh; k++) {
         vTemp = Array[k];
         if ( Array[k] == vPivot1) {
            Swap( Array[k], Array[nLow], vTemp) ;
            ++nLow;
         }
         else if ( Array[k] == vPivot2) {
            Swap( Array[k], Array[nHigh], vTemp) ;
            nHigh-- ;
            if ( Array[k] == vPivot1 ) {
               Swap( Array[k], Array[nLow], vTemp) ;
               ++nLow;
            }
         }
      }
   }

   // center part
   if (bDiffPivots) 
      DualPivotQuickSort_Asc(Array, nLow, nHigh);
}

/**
static void DualPivotQuickSort_Asc(long Array[], int nFirst, int nLast) {
   int nLen = nLast - nFirst;
   long vTemp;
   if (nLen < TINY_SIZE) { // insertion sort on tiny array
      for (int i = nFirst + 1; i <= nLast; i++) {
         for (int j = i; j > nFirst && Array[j] < Array[j - 1]; j--) {
            vTemp = Array[j - 1];
            Array[j - 1] = Array[j];
            Array[j] = vTemp;
         }
      }
      return;
   }

   // median indexes
   int sixth = nLen / 6;
   int nLeft = nFirst + sixth;
   int nL = nLeft + sixth;
   int nK = nL + sixth;
   int nG = nK + sixth;
   int nRight = nG + sixth;

   // 5-element sorting network
   if (Array[nLeft] > Array[nL]) Swap( Array[nLeft], Array[nL], vTemp );
   if (Array[nG] > Array[nRight]) Swap( Array[nG], Array[nRight], vTemp );
   if (Array[nLeft] > Array[nK]) Swap( Array[nLeft], Array[nK], vTemp );
   if (Array[nL] > Array[nK]) Swap( Array[nL], Array[nK], vTemp );
   if (Array[nLeft] > Array[nG]) Swap( Array[nLeft], Array[nG], vTemp );
   if (Array[nK] > Array[nG]) Swap( Array[nK], Array[nG], vTemp );
   if (Array[nL] > Array[nRight]) Swap( Array[nL], Array[nRight], vTemp );
   if (Array[nL] > Array[nK]) Swap( Array[nL], Array[nK], vTemp );
   if (Array[nG] > Array[nRight]) Swap( Array[nG], Array[nK], vTemp );

   // pivots: [ < vPivot1 | vPivot1 <= && <= vPivot2 | > vPivot2 ]
   long vPivot1 = Array[nL];
   long vPivot2 = Array[nG];
   bool bDiffPivots = vPivot1 != vPivot2;
   Array[nL] = Array[nFirst];
   Array[nG] = Array[nLast];

   // center part pointers
   int nLow = nFirst + 1;
   int nHigh = nLast - 1;

   // sorting
   if (bDiffPivots) {
      for (int k = nLow; k <= nHigh; k++) {
         vTemp = Array[k];
         if (vTemp < vPivot1) {
            Array[k] = Array[nLow];
            Array[nLow++] = vTemp;
         }
         else if (vTemp > vPivot2) {
            while (Array[nHigh] > vPivot2 && k < nHigh)  nHigh--;
            Array[k] = Array[nHigh];
            Array[nHigh--] = vTemp;
            
            vTemp = Array[k];
            if (vTemp < vPivot1) {
               Array[k] = Array[nLow];
               Array[nLow++] = vTemp;
            }
         }
      }
   }
   else {
      for (int k = nLow; k <= nHigh; k++) {
         vTemp = Array[k];
         if (vTemp == vPivot1) {
            continue;
         }
         if (vTemp < vPivot1) {
            Array[k] = Array[nLow];
            Array[nLow++] = vTemp;
         }
         else {
            while (Array[nHigh] > vPivot2 && k < nHigh) {
               nHigh--;
            }
            Array[k] = Array[nHigh];
            Array[nHigh--] = vTemp;
            vTemp = Array[k];
            if (vTemp < vPivot1) {
               Array[k] = Array[nLow];
               Array[nLow++] = vTemp;
            }
         }
      }
   }

   // swap
   Array[nFirst] = Array[nLow - 1];
   Array[nLow - 1] = vPivot1;

   Array[nLast] = Array[nHigh + 1];
   Array[nHigh + 1] = vPivot2;

   // nFirst and nLast parts
   DualPivotQuickSort_Asc(Array, nFirst, nLow - 2);
   DualPivotQuickSort_Asc(Array, nHigh + 2, nLast);

   // equal elements
   if (nHigh - nLow > nLen - DIST_SIZE && bDiffPivots) {
      for (int k = nLow; k <= nHigh; k++) {
         vTemp = Array[k];
         if (vTemp == vPivot1) {
            Array[k] = Array[nLow];
            Array[nLow++] = vTemp;
         }
         else if (vTemp == vPivot2) {
            Array[k] = Array[nHigh];
            Array[nHigh--] = vTemp;
            vTemp = Array[k];
            if (vTemp == vPivot1) {
               Array[k] = Array[nLow];
               Array[nLow++] = vTemp;
            }
         }
      }
   }

   // center part
   if (bDiffPivots) 
      DualPivotQuickSort_Asc(Array, nLow, nHigh);
}

  
 **/
//------------------------------------------------------------------

static void DualPivotQuickSort_Desc(long Array[], int nFirst, int nLast) {
   int nStop = nLast + 1;
   int nLen = nLast - nFirst;
   long vTemp;
   if (nLen < TINY_SIZE) { // insertion sort on tiny array
      for (int i = nFirst + 1; i < nStop; i++) {
         for (int j = i; nFirst < j  && Array[j] > Array[j - 1]; j--) 
            Swap( Array[j - 1], Array[j], vTemp );
      }
      return;
   }

   // median indexes
   int sixth = nLen / 6;
   int nLeft = nFirst + sixth;
   int nL = nLeft + sixth;
   int nK = nL + sixth;
   int nG = nK + sixth;
   int nRight = nG + sixth;

   printf("\n");
   printf(" nLeft[%d]=%d\n", nLeft, Array[nLeft] );
   printf(" nL[%d]=%d\n", nL, Array[nL] );
   printf(" nK[%d]=%d\n", nK, Array[nK] );
   printf(" nG[%d]=%d\n", nG, Array[nG] );
   printf(" nRight[%d]=%d\n", nRight, Array[nRight] );
   // 5-element sorting network
   if ( Array[nLeft] < Array[nL] ) Swap( Array[nL], Array[nLeft], vTemp );
   if ( Array[nG] < Array[nRight] ) Swap( Array[nRight], Array[nG], vTemp );
   if ( Array[nLeft] < Array[nK] ) Swap( Array[nK], Array[nLeft], vTemp );
   if ( Array[nL] < Array[nK] ) Swap( Array[nK], Array[nL], vTemp );
   if ( Array[nLeft] < Array[nG] ) Swap( Array[nG], Array[nLeft], vTemp );
   if ( Array[nK] < Array[nG] ) Swap( Array[nG], Array[nK], vTemp );
   if ( Array[nL] < Array[nRight] ) Swap( Array[nRight], Array[nL], vTemp );
   if ( Array[nL] < Array[nK] ) Swap( Array[nK], Array[nL], vTemp );
   if ( Array[nG] < Array[nRight] ) Swap( Array[nRight], Array[nG], vTemp );
   
   

   printf("\n");
   printf(" nLeft[%d]=%d\n", nLeft, Array[nLeft] );
   printf(" nL[%d]=%d\n", nL, Array[nL] );
   printf(" nK[%d]=%d\n", nK, Array[nK] );
   printf(" nG[%d]=%d\n", nG, Array[nG] );
   printf(" nRight[%d]=%d\n", nRight, Array[nRight] );
   
   // pivots: [ < vPivot1 | vPivot1 <= && <= vPivot2 | > vPivot2 ]
   long vPivot1 = Array[nL];
   long vPivot2 = Array[nG];
   bool bDiffPivots = vPivot1 != vPivot2;
   
   Array[nL] = Array[nFirst];
   Array[nG] = Array[nLast];

   // center part pointers
   int nLow = nFirst + 1;
   int nHigh  = nLast - 1;

   // sorting
   if (bDiffPivots) {
      for (int k = nLow; k <= nHigh; k++) {
         if (Array[k] > vPivot1) {
            Swap( Array[k], Array[nLow], vTemp) ;
            ++nLow;
         } 
         else if ( Array[k] < vPivot2) {
            while (Array[nHigh] < vPivot2 && k < nHigh)  --nHigh;
            Swap( Array[k], Array[nHigh], vTemp );
            --nHigh;
            
            if ( Array[k] > vPivot1) {
               Swap( Array[k], Array[nLow], vTemp);
               ++nLow;
            }
         }
      }
   } else {
      for (int k = nLow; k <= nHigh; k++) {
         if ( Array[k] == vPivot1)  continue;
         if ( Array[k] > vPivot1) {
            Swap( Array[k], Array[nLow], vTemp);
            ++nLow;
         }
         else {
            while( Array[nHigh] < vPivot2 && k < nHigh ) --nHigh;
            Swap( Array[k], Array[nHigh], vTemp ) ;
            --nHigh;
            
            if (Array[k] > vPivot1) {
               Swap( Array[k], Array[nLow], vTemp) ;
               ++nLow ;
            }
         }
      }
   }

   // swap
   Array[nFirst] = Array[nLow - 1];
   Array[nLow - 1] = vPivot1;

   Array[nLast] = Array[nHigh + 1];
   Array[nHigh + 1] = vPivot2;

   // nFirst and nLast parts
   DualPivotQuickSort_Asc(Array, nFirst, nLow - 2);
   DualPivotQuickSort_Asc(Array, nHigh + 2, nLast);

   // equal elements
   if (nHigh - nLow > nLen - DIST_SIZE && bDiffPivots) {
      for (int k = nLow; k <= nHigh; k++) {
         vTemp = Array[k];
         if ( Array[k] == vPivot1) {
            Swap( Array[k], Array[nLow], vTemp) ;
            ++nLow;
         }
         else if ( Array[k] == vPivot2) {
            Swap( Array[k], Array[nHigh], vTemp) ;
            nHigh-- ;
            if ( Array[k] == vPivot1 ) {
               Swap( Array[k], Array[nLow], vTemp) ;
               ++nLow;
            }
         }
      }
   }

   // center part
   if (bDiffPivots) 
      DualPivotQuickSort_Asc(Array, nLow, nHigh);
}

//------------------------------------------------------------------

