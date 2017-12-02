#include "ebm_base.h"

const size_t gnByteLong = sizeof(long); /* must be a power of 2 */

// #define exch(a, b, t) (t = a, a = b, b = t)

// #define swap(a, b) \
   // swaptype != 0 ? swapfunc(a, b, _nBytes, swaptype)  \
                 // : (void)exch(*(long*)(a), *(long*)(b), t)

// static void swapfunc( char *ArrayPos1, char *ArrayPos2, size_t nItems, int swaptype ) {
   // if (swaptype <= 1) {
      // long t;
      // for( ; 0 < nItems ; --nItems, ArrayPos1 += gnByteLong, ArrayPos2 += gnByteLong )
         // Swap(*(long*)ArrayPos1, *(long*)ArrayPos2, t);
   // } else {
      // char t;
      // for( ; 0 < nItems-- ; ++ArrayPos1,  ++ArrayPos2 )
         // Swap(*ArrayPos1, *ArrayPos2, t);
   // }
// }
static void SwapRange_c( void* p_pArrayPos1, void* p_pArrayPos2, size_t nItems ) {
   byte *pArrayPos1 = (byte*)p_pArrayPos1,
        *pArrayPos2 = (byte*)p_pArrayPos2;
   long vTmp;
   for( ; 0 < nItems ; --nItems, pArrayPos1 += gnByteLong, pArrayPos2 += gnByteLong )
      Swap( *(long*)pArrayPos1, *(long*)pArrayPos2, vTmp );
}

// static char *med3( char *a, char *b, char *c, int (*fnCompare)() ) { 
   // return fnCompare(a, b) < 0 
          // ? (fnCompare(b, c) < 0 ? b : fnCompare(a, c) < 0 ? c : a)
          // : (fnCompare(b, c) > 0 ? b : fnCompare(a, c) > 0 ? c : a);
// }

static int median3( long* p_Array, int a, int b, int c ) { 
   return   p_Array[a] < p_Array[b]
          ? ( p_Array[b] < p_Array[c] ? b : p_Array[a] < p_Array[c] ? c : a )
          : ( p_Array[b] > p_Array[c] ? b : p_Array[a] > p_Array[c] ? c : a );
}

void InsertSort_Asc(long* Array, int nFirst, int nLast) {
   int nStop = nLast +1;
   long vTemp;
   for (int i = nFirst + 1; i < nStop; i++) 
      for (int j = i; nFirst < j  && Array[j] < Array[j - 1]; j--) 
         Swap( Array[j - 1], Array[j], vTemp );
}

#define Min2Arr( _Array, _nIdx1, _nIdx2 ) \
   _Array[(_nIdx1)] < _Array[(_nIdx2)] ? (_nIdx1) : (_nIdx2)

// void qsort3way_bently( 
     // long *Array
   // , size_t p_nSlots
   // , size_t p_nBytes
// )  {
#define qsort3way(   _Array, _nLength, ...) csname2(QSort3way_bently_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define QSort3way_bently_0( _Array, _nLength)                        QSort3way_bently( _Array, _nLength, ASC )
#define QSort3way_bently_1( _Array, _nLength, _bIsAscending)         QSort3way_bently( _Array, _nLength, (_bIsAscending) )
#define QSort3way_bently_2( _Array, _nLength, _Type, _bIsAscending)  QSort3way_bently( _Array, _nLength, (_bIsAscending) )

void Qsort3way_Asc( long *Array, int p_nFirst, int p_nLast) ;

inline void QSort3way_bently(long* p_Array, size_t nLength, bool p_bAscending ) {
   if (nLength < 2) return;
   if( p_bAscending )
      Qsort3way_Asc(p_Array, 0, nLength-1);
   else
      Qsort3way_Asc(p_Array, 0, nLength-1);
}
   
void Qsort3way_Asc( long *Array, int p_nFirst, int p_nLast )  {
   int nSlots = p_nLast - p_nFirst + 1,
       nStop  = p_nLast + 1;
   int swaptype;
   // char *pStop = Array + p_nSlots * p_nBytes ;
   long vTmp ;
   int nBytes = sizeof(long);
   size_t nSpeudoMedian = 0;

   // swaptype = (Array-(char*)0 | nBytes) % gnByteLong 
              // ? 2 
              // : nBytes > gnByteLong ? 1 : 0 ;
   
   int nPL, nPM, nPN ;

   //-------------------[ Insertion sort on smallest arrays ]----------------
   if( nSlots < 7 ) { 
      for( int i = p_nFirst+1 ; i < nStop ; ++i  )
         for( int j = i ; p_nFirst < j && Array[j-1] > Array[j] ; --j )
            Swap( Array[j-1], Array[j], vTmp );
      return;
   }
   // printf(" *****- ----------- -****\n");
   
   //-------------------[ Median of Three ]----------------
   int nMiddle = p_nFirst + nSlots / 2 ; // For Small arrays, middle element (nMiddle)
   if( nSlots > 7 ) {
      nPL = p_nFirst ;
      nPN = p_nLast ;
      if( nSlots > 40 ) {     // Big arrays, pseudomedian of 9 
         nSpeudoMedian = (nSlots/8);
         nPL    = median3( Array, p_nFirst,                 p_nFirst+nSpeudoMedian, p_nFirst+2*nSpeudoMedian );
         nMiddle= median3( Array, nMiddle-nSpeudoMedian,    nMiddle,                nMiddle+nSpeudoMedian );
         nPN    = median3( Array, p_nLast-2*nSpeudoMedian,  p_nLast-nSpeudoMedian,  p_nLast );
      }
      nMiddle = median3( Array, nPL, nMiddle, nPN ); // Mid-size, med of 3 
   } 

   int nPivot;
   if (swaptype != 0) {  // pv points to partition value 
      nPivot = p_nFirst ;
      Swap( Array[nPivot], Array[nMiddle], vTmp ); 
   } else {
      nPivot = nMiddle ;
   }
   //-------------------[ Quicksort ]----------------
   // pa = pb = Array;
   // pc = pd = Array + (p_nSlots-1)*p_nBytes;
   int nLow  = p_nFirst, 
       nLow2 = p_nFirst ;
   int nHigh  = p_nLast,
       nHigh2 = p_nLast ;
       
   for (;;) {
      while( nLow < nStop &&  Array[nLow] <= Array[nPivot] ) {
         if( Array[nLow] == Array[nPivot] ) { 
            Swap( Array[nLow], Array[nLow2], vTmp );
            ++nLow2 ;
         }
         ++nLow ;
      }
      while( nLow <= nHigh  && Array[nPivot] <= Array[nHigh] ) {
         if( Array[nPivot] == Array[nHigh] ) { 
            Swap( Array[nHigh], Array[nHigh2], vTmp ); 
            --nHigh2 ; 
         }
         --nHigh ;
      }
      if( nLow > nHigh ) break;
      Swap( Array[nLow], Array[nHigh], vTmp );
      ++nLow ;
      --nHigh ;
   }
   // p_nSlots = Array + p_nSlots * p_nBytes;
// printf(" p_nFirst=%d\t p_nLast=%d\tMiddle=%d\tnPivot=%d\n", p_nFirst, p_nLast, nMiddle, nPivot);
// printf(" nSpeudoMedian=%d\t nSpeudo-1=%d\tnLast+1-nSpeudoM=%d\n\n", nSpeudoMedian, nSpeudoMedian-1, p_nLast+1-nSpeudoMedian);
   
   nSpeudoMedian = Min2Arr( Array, nLow2, nLow-nLow2 );
// printf(" nSpeudoMedian=%d\t nSpeudo-1=%d\tnLast+1-nSpeudoM=%d\n\n", nSpeudoMedian, nSpeudoMedian-1, p_nLast+1-nSpeudoMedian);
   if (nSpeudoMedian > 0) 
      SwapRange_c(  Array
                  , Array + nLow - nSpeudoMedian
                  , nSpeudoMedian ) ;
   
   nSpeudoMedian = Min2Arr(Array, nHigh2-nHigh,    nSlots-nHigh2-1) ;
// printf(" nSpeudoMedian=%d\t nSpeudo-1=%d\tnLast+1-nSpeudoM=%d\n\n", nSpeudoMedian, nSpeudoMedian-1, p_nLast+1-nSpeudoMedian);
   if( nSpeudoMedian > 0) 
      SwapRange_c(  Array + nLow
                  , Array + nSlots-nSpeudoMedian
                  , nSpeudoMedian ) ;


   // if( (nSpeudoMedian = nLow   - nLow2) > 1 )  Qsort3way_Asc(Array, p_nFirst, nSpeudoMedian-1 );
// printf(" nSpeudoMedian=%d\t nSpeudo-1=%d\tnLast+1-nSpeudoM=%d\n\n", nSpeudoMedian, nSpeudoMedian-1, p_nLast+1-nSpeudoMedian);
   // if( (nSpeudoMedian = nHigh2 - nHigh) > 1 )  Qsort3way_Asc(Array, p_nLast-nSpeudoMedian+1, p_nLast );
// printf(" nSpeudoMedian=%d\t nSpeudo-1=%d\tnLast+1-nSpeudoM=%d\n\n", nSpeudoMedian, nSpeudoMedian-1, p_nLast+1-nSpeudoMedian);
// printf("  -------******* --------\n");
}

//***********************************************************************
const size_t gnByteLong = sizeof(long); // must be a power of 2 

#include <stddef.h>

typedef long WORD;

// #define min(a,b) ( (a) < (b) ? (a) : (b) )

#define W sizeof(WORD) /* must be a power of 2 *//

#define SWAPINIT(a, es) swaptype = \
   (a-(char*)0 | es) % W ? 2 : es > W ? 1 : 0

#define vecswap(a, b, n) \
   if (n > 0) \
      swapfunc(a, b, n, swaptype)
      
#define exch(a, b, t) (t = a, a = b, b = t)

#define swap(a, b) \
   swaptype != 0 ? swapfunc(a, b, es, swaptype)  \
                 : (void)exch(*(WORD*)(a), *(WORD*)(b), t)

static void swapfunc(char *a, char *b, size_t n, int swaptype) {
   if (swaptype <= 1) {
      WORD t;
      for( ; n > 0; a += W, b += W, n -= W)
         exch(*(WORD*)a, *(WORD*)b, t);
   } else {
      char t;
      for( ; n > 0; a += 1, b += 1, n -= 1)
         exch(*a, *b, t);
   }
}

#define PVINIT(pv, pm) \
   if (swaptype != 0) \
      pv = a, swap(pv, pm); \
   else \
      pv = (char*)&v, v = *(WORD*)pm

static char *med3( char *a, char *b, char *c, int (*cmp)() ) {
 return cmp(a, b) < 0 
        ? (cmp(b, c) < 0 ? b : cmp(a, c) < 0 ? c : a)
        : (cmp(b, c) > 0 ? b : cmp(a, c) > 0 ? c : a);
}      

#define qsort3way(   _Array, _nLength, ...) csname2(qsort3way_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define qsort3way_0( _Array, _nLength)                        _QSort3Way( (char*)_Array, _nLength, sizeof(*(_Array)), FNCOMPARE(long) )
#define qsort3way_1( _Array, _nLength, _bIsAscending)         _QSort3Way( (char*)_Array, _nLength, sizeof(*(_Array)), FNCOMPARE(long))
#define qsort3way_2( _Array, _nLength, _Type, _bIsAscending)  _QSort3Way( (char*)_Array, _nLength, sizeof(*(_Array)), FNCOMPARE(_Type) )


void _QSort3Way(char *a, size_t n, size_t es, int (*cmp)()) {
   char *pa, *pb, *pc, *pd, *pl, *pm, *pn, *pv;
   int r, swaptype;
   WORD t, v;
   size_t s;
   SWAPINIT(a, es);
   if (n < 7) { /* Insertion sort on smallest arrays */
      for (pm = a + es; pm < a + n*es; pm += es)
         for (pl = pm; pl > a && cmp(pl-es, pl) > 0; pl -= es)
            swap(pl, pl-es);
      return;
   }
   pm = a + (n/2)*es; /* Small arrays, middle element */
   if (n > 7) {
      pl = a;
      pn = a + (n-1)*es;
      if (n > 40) { /* Big arrays, pseudomedian of 9 */
         s = (n/8)*es;
         pl = med3(pl, pl+s, pl+2*s, cmp);
         pm = med3(pm-s, pm, pm+s, cmp);
         pn = med3(pn-2*s, pn-s, pn, cmp);
      }
      pm = med3(pl, pm, pn, cmp); /* Mid-size, med of 3 */
   }
   PVINIT(pv, pm); /* pv points to partition value */
   pa = pb = a;
   pc = pd = a + (n-1)*es;
   for (;;) {
      while (pb <= pc && (r = cmp(pb, pv)) <= 0) {
         if (r == 0) { 
            swap(pa, pb); 
            pa += es; 
         }
         pb += es;
      }
      while (pc >= pb && (r = cmp(pc, pv)) >= 0) {
         if (r == 0) { 
            swap(pc, pd); 
            pd -= es; 
         }
         pc -= es;
      }
      if (pb > pc) break;
      swap(pb, pc);
      pb += es;
      pc -= es;
   }
   pn = a + n*es;
   s = min(pa-a, pb-pa ); 
   vecswap(a, pb-s, s);
   s = min(pd-pc, pn-pd-es); 
   vecswap(pb, pn-s, s);
   if ((s = pb-pa) > es) _QSort3Way(a, s/es, es, cmp);
   if ((s = pd-pc) > es) _QSort3Way(pn-s, s/es, es, cmp);
}

  
 **************************************************************/