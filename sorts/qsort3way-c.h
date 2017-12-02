
/*______________________________________________________________________________
  |                                                                            |__
  | Objetive: Implementation of QuickSor3ways optimized using ANSI C99                   | |
  |           This code is my  adaptation from Robert Sedgewick's code in Java | |
  |           This Code is more effiente that original Robert Sedgewick'scode  | |
  |  Versión: 1.0 Crossover Platform.                                          | |
  |   Author: Edgard Bernardo  Medina Quispe, <edgardmedina@yahoo.com>         | |
  |  License: Copyright 2011-2016. Arequipa, Perú.                             | |
  |                                                                            | |
  |                                                                            | |
  |                                                                            | |
  |____________________________________________________________________________| |
    |____________________________________________________________________________|*/

#ifndef __qsort3way_h__
#define __qsort3way_h__

//-------------------------------------------------------------------------
/*
#define SWAP(a, b) Swap_C(   (void*)&(a)\
                           , (void*)&(b)\
                           , ( char[ (sizeof(a) == sizeof(b)) \
                               ? (ptrdiff_t)sizeof(a) \
                               : -1]\
                             ) {0}\
                           , sizeof(a) \
                         )
inline void Swap_C(void* ptr1, void* ptr2, void* pTmp, size_t p_nSize) {
   memcpy(pTmp, ptr1, p_nSize);
   memcpy(ptr1, ptr2, p_nSize);
   memcpy(ptr2 , pTmp, p_nSize);
}
///-------------------------------------------------------------------------
#define swap(a, b, t) (t = a, a = b, b = t)
// #define swap_c_( ptr1, ptr2, _t)   (void)swap( *(long*)(ptr1), *(long*)(ptr2), _t )
// #define swap_c_( ptr1, ptr2, _t)   (void)swap( *(long*)(ptr1), *(long*)(ptr2), _t )
// -------------------------------------------------------------------------

// #define SWAP( __a, __b, __t, __n )  (void)swap( *(long*)(__a), *(long*)(__b), __t )
// #define SWAP( __a, __b, __t, __n )  (void)swap( *(long*)(__a), *(long*)(__b), *(long*)(__t) )
// #define SWAP( __a, __b, __t, __n ) Swap_C(  (__a), (__b), (__t), (__n) )
// #define SWAP( __a, __b, __t, __n ) Swap(    (__a), (__b), (__t), (__n) )
/**/
// -------------------------------------------------------------------------

static Byte *med3( Byte *pVal1, Byte *pVal2, Byte *pVal3, fnCompare_t fnCompare ) {
 return fnCompare( pVal1, pVal2 ) < 0 
        ? (fnCompare( pVal2, pVal3 ) < 0 ? pVal2 : fnCompare( pVal1, pVal3 ) < 0 ? pVal3 : pVal1 )
        : (fnCompare( pVal2, pVal3 ) > 0 ? pVal2 : fnCompare( pVal1, pVal3 ) > 0 ? pVal3 : pVal1 );
}      

#define qsort3way(   _Array, _nLength, ...) csname2(qsort3way_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define qsort3way_0( _Array, _nLength)                        _QSort3Way( (Byte*)_Array, _nLength, sizeof(*(_Array)), FNCOMPARE(long) )
#define qsort3way_1( _Array, _nLength, _bIsAscending)         _QSort3Way( (Byte*)_Array, _nLength, sizeof(*(_Array)), FNCOMPARE(long))
#define qsort3way_2( _Array, _nLength, _Type, _bIsAscending)  _QSort3Way( (Byte*)_Array, _nLength, sizeof(*(_Array)), FNCOMPARE(_Type) )

void _QSort3Way(Byte *p_pArray, size_t p_nSlots, size_t p_nSizeBytes, fnCompare_t fnCompare ) {
   Byte   *pStop  = p_pArray + p_nSlots * p_nSizeBytes 
        , *pLeft  = p_pArray
        , *pRight = pStop - p_nSizeBytes ;

   long   v;
   // long  pTmp ;
   Byte pTmp[p_nSizeBytes];
   if( p_nSlots < 3 ) {
      if( p_nSlots == 2 ) 
         if( fnCompare( pLeft, pRight ) > 0 )
            Swap( pLeft, pRight, pTmp, p_nSizeBytes);
      return ;
   }
   
   // Insertion sort on smallest arrays 
   if( p_nSlots < 7 ) { 
      for( Byte* pIter = pLeft + p_nSizeBytes ; pIter < pStop ; pIter += p_nSizeBytes )
         for( Byte* pIter2 = pIter; pLeft < pIter2  && fnCompare(pIter2-p_nSizeBytes, pIter2) > 0; pIter2 -= p_nSizeBytes)
            Swap( pIter2, pIter2-p_nSizeBytes, pTmp, p_nSizeBytes );
      return;
   }
   
   Byte  *pPivot,
         *pMedian  = pLeft + (p_nSlots/2)*p_nSizeBytes; // Small arrays, middle element 
   if (p_nSlots > 7) {
      Byte  *pLMedian = pLeft
          , *pRMedian = pRight ;
      if( p_nSlots > 40 ) { // Big arrays, pseudomedian of 9 
         size_t nSpeudoMedian = (p_nSlots/8) * p_nSizeBytes;
         pLMedian = med3(pLMedian,                 pLMedian+nSpeudoMedian, pLMedian+2*nSpeudoMedian, fnCompare);
         pMedian  = med3(pMedian-nSpeudoMedian,    pMedian,                pMedian+nSpeudoMedian,    fnCompare);
         pRMedian = med3(pRMedian-2*nSpeudoMedian, pRMedian-nSpeudoMedian, pRMedian,                 fnCompare);
      }
      pMedian = med3(pLMedian, pMedian, pRMedian, fnCompare); // Mid-size, med of 3 
   }
   pPivot = (Byte*)&v, 
        v = *(long*)pMedian;
      
   int nAnsCmp;
   Byte  *pLow   = pLeft
       , *pLow2  = pLeft
       , *pHigh  = pRight
       , *pHigh2 = pRight ;
        
   for(;;) {
      while( pLow2 < pStop && (nAnsCmp = fnCompare(pLow2, pPivot)) <= 0) {
         if( nAnsCmp == 0 ) { 
            Swap( pLow, pLow2, pTmp, p_nSizeBytes); 
            pLow += p_nSizeBytes; 
         }
         pLow2 += p_nSizeBytes;
      }
      while( pLow2 <= pHigh && (nAnsCmp = fnCompare(pHigh, pPivot)) >= 0) {
         if( nAnsCmp == 0 ) { 
            Swap( pHigh, pHigh2, pTmp, p_nSizeBytes );
            pHigh2 -= p_nSizeBytes;
         }
         pHigh -= p_nSizeBytes;
      }
      if( pLow2 > pHigh ) break;
      Swap( pLow2, pHigh, pTmp, p_nSizeBytes );
      pLow2 += p_nSizeBytes;
      pHigh -= p_nSizeBytes;
   }
   
   int nItems;
   if( (nItems = Min2(pLow-pLeft, pLow2-pLow ) / p_nSizeBytes) > 0 )
      for( Byte  *a=pLeft
               , *b=pLow2 - nItems * p_nSizeBytes 
          ; nItems > 0
          ; a += p_nSizeBytes, b += p_nSizeBytes, --nItems  )
         Swap( a, b, pTmp, p_nSizeBytes );

   if( (nItems = Min2(pHigh2-pHigh, pStop-pHigh2-p_nSizeBytes) / p_nSizeBytes) > 0 )
      for( Byte  *a= pLow2
               , *b= pStop - nItems * p_nSizeBytes
           ; nItems > 0
           ; a += p_nSizeBytes, b += p_nSizeBytes, --nItems )
         Swap( a, b, pTmp, p_nSizeBytes );
         
   if( (nItems = (pLow2 -pLow )/p_nSizeBytes) > 1 ) _QSort3Way(pLeft,                     nItems, p_nSizeBytes, fnCompare);
   if( (nItems = (pHigh2-pHigh)/p_nSizeBytes) > 1 ) _QSort3Way(pStop-nItems*p_nSizeBytes, nItems, p_nSizeBytes, fnCompare);
}

//-------------------------------------------------------------------------
/*
typedef long long;


// #define W sizeof(long) // must be p_pArray power of 2 

// #define SWAPINIT(p_pArray, p_nSizeBytes) swaptype = \
   // (p_pArray-(char*)0 | p_nSizeBytes) % W ? 2 : p_nSizeBytes > W ? 1 : 0

#define vecswap(p_pArray, b, p_nSlots, _nSizeBytes) \
   if (p_nSlots > 0) \
      swapfunc(p_pArray, b, p_nSlots, _nSizeBytes, swaptype)
      
// #define exch( _a, _b, _t) ( _t = _a, _a = _b, _b = _t )


// #define swap_c(p_pArray, b) \
   // swaptype != 0 ? swapfunc(p_pArray, b, p_nSizeBytes, swaptype)  \
                 // : (void)exch(*(long*)(p_pArray), *(long*)(b), vTmp)

static void swapfunc(Byte* p_pArray, Byte* p_Array2, size_t p_nSlots, size_t p_nSizeBytes, int swaptype) {
   Byte* pTmp = (Byte*)mem_alloc(1, p_nSizeBytes) ;
   if (swaptype <= 1) {
      for( ; 0 < p_nSlots ; p_pArray += p_nSizeBytes, p_Array2 += p_nSizeBytes, p_nSlots -= p_nSizeBytes)
         Swap( p_pArray, p_Array2, p_nSizeBytes, pTmp );
   } else {
      Byte vTmp;
      for( ; p_nSlots > 0; p_pArray += 1, p_Array2 += 1, p_nSlots -= 1)
         Swap(*p_pArray, *p_Array2, vTmp);
   }
   mem_free(pTmp) ;
}

#define PVINIT(pPivot, pMedian) \
   if (swaptype != 0) \
      pPivot = p_pArray, swap_c(pPivot, pMedian); \
   else \
      pPivot = (char*)&v, v = *(long*)pMedian

static char *med3( char *p_pArray, char *b, char *c, int (*fnCompare)() ) {
 return fnCompare(p_pArray, b) < 0 
        ? (fnCompare(b, c) < 0 ? b : fnCompare(p_pArray, c) < 0 ? c : p_pArray)
        : (fnCompare(b, c) > 0 ? b : fnCompare(p_pArray, c) > 0 ? c : p_pArray);
}      

#define qsort3way(   _Array, _nSlots, ...) csname2(qsort3way_, Count_Args(__VA_ARGS__))( (_Array), (_nSlots), ##__VA_ARGS__)
#define qsort3way_0( _Array, _nSlots)                        _QSort3Way( (Byte*)_Array, _nSlots, sizeof(*(_Array)), FNCOMPARE(long) )
#define qsort3way_1( _Array, _nSlots, _bIsAscending)         _QSort3Way( (Byte*)_Array, _nSlots, sizeof(*(_Array)), FNCOMPARE(long))
#define qsort3way_2( _Array, _nSlots, _Type, _bIsAscending)  _QSort3Way( (Byte*)_Array, _nSlots, sizeof(*(_Array)), FNCOMPARE(_Type) )


void _QSort3Way(Byte* p_pArray, size_t p_nSlots, size_t p_nSizeBytes, int (*fnCompare)() ) {
   if( p_nSlots < 2 ) return ;
   char *pLow, *pLow2, *pHigh, *pHigh2, *pLeft, *pMedian, *pRight, *pPivot;
   int nCmpAns, 
       swaptype = (p_pArray-(char*)0 | p_nSizeBytes) % p_nSizeBytes ? 2 : p_nSizeBytes > p_nSizeBytes ? 1 : 0 ;
   long  vTmp, v;
   size_t s;
   
   Byte  *pIter  ,
         *pIter2 ,
         *pStop = p_pArray + p_nSlots * p_nSizeBytes,
         *pTmp = (Byte*)mem_alloc(1, p_nSizeBytes );

   if (p_nSlots < 7) { // Insertion sort on smallest arrays 
      for ( pIter = p_pArray + p_nSizeBytes; pIter < pStop ; pIter += p_nSizeBytes)
         for ( pIter2 = pIter ; p_pArray < pIter2 && fnCompare( pIter2-p_nSizeBytes, pIter2) > 0 ; pIter2 -= p_nSizeBytes)
            Swap( pIter2, pIter2-p_nSizeBytes, p_nSizeBytes, pTmp );
      mem_free(pTmp);
      return;
   }
   pMedian = p_pArray + (p_nSlots/2)*p_nSizeBytes; // Small arrays, middle element 
   if (p_nSlots > 7) {
      pLeft = p_pArray;
      pRight = p_pArray + (p_nSlots-1)*p_nSizeBytes;
      if (p_nSlots > 40) { // Big arrays, pseudomedian of 9 
         s = (p_nSlots/8)*p_nSizeBytes;
         pLeft = med3(pLeft, pLeft+s, pLeft+2*s, fnCompare);
         pMedian = med3(pMedian-s, pMedian, pMedian+s, fnCompare);
         pRight = med3(pRight-2*s, pRight-s, pRight, fnCompare);
      }
      pMedian = med3(pLeft, pMedian, pRight, fnCompare); // Mid-size, med of 3 
   }
   // pPivot points to partition value 
   if (swaptype != 0) 
      pPivot = p_pArray, Swap(pPivot, pMedian, p_nSizeBytes, pTmp); 
   else 
      pPivot = (char*)&v, v = *(long*)pMedian;
   
   pLow = pLow2 = p_pArray;
   pHigh = pHigh2 = p_pArray + (p_nSlots-1)*p_nSizeBytes;
   for (;;) {
      while (pLow2 <= pHigh && ( nCmpAns = fnCompare(pLow2, pPivot) ) <= 0) {
         if( nCmpAns == 0 ) { 
            Swap(pLow, pLow2, p_nSizeBytes, pTmp); 
            pLow += p_nSizeBytes; 
         }
         pLow2 += p_nSizeBytes;
      }
      while (pHigh >= pLow2 && (nCmpAns = fnCompare(pHigh, pPivot)) >= 0) {
         if (nCmpAns == 0) { 
            Swap(pHigh, pHigh2, p_nSizeBytes, pTmp); 
            pHigh2 -= p_nSizeBytes; 
         }
         pHigh -= p_nSizeBytes;
      }
      if (pLow2 > pHigh) break;
      Swap(pLow2, pHigh, p_nSizeBytes, pTmp); 
      pLow2 += p_nSizeBytes;
      pHigh -= p_nSizeBytes;
   }
   pRight = p_pArray + p_nSlots*p_nSizeBytes; 
   s = Min2(pLow-p_pArray, pLow2-pLow ); 
   vecswap(p_pArray, pLow2-s, s, p_nSizeBytes);
   s = Min2(pHigh2-pHigh, pRight-pHigh2-p_nSizeBytes); 
   vecswap(pLow2, pRight-s, s, p_nSizeBytes);
   if ((s = pLow2-pLow) > p_nSizeBytes) _QSort3Way(p_pArray, s/p_nSizeBytes, p_nSizeBytes, fnCompare);
   if ((s = pHigh2-pHigh) > p_nSizeBytes) _QSort3Way(pRight-s,     s/p_nSizeBytes, p_nSizeBytes, fnCompare);
   mem_free(pTmp);
}

/**/
/*
inline void Swap_C(void* ptr1, void* ptr2, void* pTmp, size_t p_nSize) {
   memcpy(pTmp,  ptr1, p_nSize);
   memcpy(ptr1,  ptr2, p_nSize);
   memcpy(ptr2 , pTmp, p_nSize);
}
#define SWAP(a, b) Swap_C(   (void*)&(a)\
                           , (void*)&(b)\
                           , ( char[ (sizeof(a) == sizeof(b)) \
                               ? (ptrdiff_t)sizeof(a) \
                               : -1]\
                             ) {0}\
                           , sizeof(a) \
                         )
 */
 
#endif // #define __qsort3way_h__
