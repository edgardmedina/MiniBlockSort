/*****************************************************************************************************
 * @Algorith   : Quicksort.
 *               Generic adaptation in 'C' pure, to works in 'C' and/or C++ from:
 *                 QSortAlgorithm.java	1.6f 95/01/31 James Gosling
 *                 Copyright (c) 1994-1995 Sun Microsystems, Inc. All Rights Reserved.
 *                 @author       : James Gosling
 *                 @version      : 1.6f, 31 Jan 1995
 *                 @last modified: 09 Jan 1998
 * 
 * @Adapted by : Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, Perú.
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
 * QuickSort(): QuickSort algorithm when specified only the basic <Type>.
 *              Previously must declare the functor name with a format:
 * 
 *                   int <Type>_Compare(const void*, const void*);
 * 
 * Syntax:  void QuickSort( 
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
 * QuickSort_fn(): QuickSort algorithm when specified a funtor compare.
 *                 the funtor can has any name.
 *                   int <Type>fnCompare(const void*, const void*);
 * 
 * Syntax:  void QuickSort_fn( 
 *                           <Type>*     pArray        // Valid Array to sort.
 *                          ,size_t      p_nLength     // Length of array.
 *                          ,fnCompare_t fnCompare     // Funtor Compare
 *                          , bool       p_IsAscending // Boolean Flag
 *                                                     //   - true : the sortering is ascending.
 *                                                     //   - false: the sortering is descending.
 *                         )                           // Returns: the array sorted.
 * 
 *****************************************************************************************************/
 
static void QSort_Asc_c(  void* p_pFirst, void* p_pLast, size_t p_nSizeBytes, fnCompare_t fnCompare ) ;
static void QSort_Desc_c( void* p_pFirst, void* p_pLast, size_t p_nSizeBytes, fnCompare_t fnCompare ) ;

#define QuickSort_c(  _pArray, _nSlots, _Type, ... )       csname2(QuickSort_fn_, Count_Args(__VA_ARGS__))( _pArray, (_nSlots), FNCOMPARE(_Type), ##__VA_ARGS__)
#define QuickSort_fn( _pArray, _nSlots, _fnCompare, ... )  csname2(QuickSort_fn_, Count_Args(__VA_ARGS__))( _pArray, (_nSlots), _fnCompare, ##__VA_ARGS__)

#define QuickSort_fn_0( _pArray, _nSlots, _fnCompare )                 _QuickSort_fn( (void*)_pArray, _nSlots, sizeof(*_pArray), _fnCompare, ASC ) 
#define QuickSort_fn_1( _pArray, _nSlots, _fnCompare, _bIsAscending )  _QuickSort_fn( (void*)_pArray, _nSlots, sizeof(*_pArray), _fnCompare, _bIsAscending ) 

inline void  _QuickSort_fn( void* pArray, size_t nSlots, size_t p_nSizeBytes, fnCompare_t fnCompare, bool bIsAscending ) {
   if (nSlots < 2) return;
   if( bIsAscending ) 
      QSort_Asc_c(  pArray, pArray+(nSlots-1)*p_nSizeBytes, p_nSizeBytes, fnCompare );
   else
      QSort_Desc_c( pArray, pArray+(nSlots-1)*p_nSizeBytes, p_nSizeBytes, fnCompare );
}

// ...............................................

static void QSort_Asc_c( void* p_pFirst, void* p_pLast, size_t p_nSizeBytes, fnCompare_t fnCompare ) {
   if (p_pFirst >= p_pLast)  return;

   void* pTmp = mem_alloc(1,p_nSizeBytes);
   
   if( p_pFirst == (p_pLast - p_nSizeBytes) ) {
      if( fnCompare(p_pFirst, p_pLast) > 0 ) 
         Swap( p_pFirst, p_pLast, pTmp, p_nSizeBytes );
      mem_delete(pTmp);
      return;
   }
   
   // ------ Partition ------
   int factor = p_nSizeBytes * (((p_pLast -p_pFirst)/p_nSizeBytes+1)%2 == 0 ? 1 : 0) ;
   
   void* pMidle = p_pFirst + ((size_t)p_pLast - (size_t)p_pFirst - factor ) / 2 ;
   void* pLow   = p_pFirst;
   void* pHigh  = p_pLast;
   void* pPivot = mem_alloc(1, p_nSizeBytes);
   
   Swap( pMidle, p_pLast, pPivot, p_nSizeBytes );

   while( pLow < pHigh ) {
      while ( fnCompare( pLow, pPivot)  < 1 && pLow < pHigh ) pLow  += p_nSizeBytes;
      while ( fnCompare( pPivot, pHigh) < 1 && pLow < pHigh ) pHigh -= p_nSizeBytes;
      
      if( pLow < pHigh ) 
         Swap( pLow, pHigh, pTmp, p_nSizeBytes);
   }
   // In this point: pLow == pHigh
   mem_copy_c(p_pLast, pHigh, p_nSizeBytes);
   mem_copy_c(pHigh, pPivot, p_nSizeBytes);
   
   mem_delete(pTmp);
   mem_delete(pPivot);
   // ------ End Partition ------
   
   QSort_Asc_c( p_pFirst,           pLow-p_nSizeBytes, p_nSizeBytes, fnCompare );
   QSort_Asc_c( pHigh+p_nSizeBytes, p_pLast,           p_nSizeBytes, fnCompare );
}

// ...............................................

static void QSort_Desc_c( void* p_pFirst, void* p_pLast, size_t p_nSizeBytes, fnCompare_t fnCompare ) {
   if (p_pFirst >= p_pLast)  return;
   
   void* pTmp = mem_alloc(1,p_nSizeBytes);
   
   if( p_pFirst == (p_pLast - p_nSizeBytes) ) {
      if( fnCompare(p_pFirst, p_pLast) < 0 ) 
         Swap( p_pFirst, p_pLast, pTmp, p_nSizeBytes );
      mem_delete(pTmp);
      return;
   }
   
   // ------ Partition ------
   int factor = p_nSizeBytes * (((p_pLast -p_pFirst)/p_nSizeBytes+1)%2 == 0 ? 1 : 0) ;
   
   void* pMidle = p_pFirst + ((size_t)p_pLast - (size_t)p_pFirst - factor ) / 2 ;
   void* pLow   = p_pFirst;
   void* pHigh  = p_pLast;
   void* pPivot = mem_alloc(1, p_nSizeBytes);
   
   Swap( pMidle, p_pLast, pPivot, p_nSizeBytes );

   while( pLow < pHigh ) {
      while ( fnCompare( pLow, pPivot)  > -1 && pLow < pHigh ) pLow += p_nSizeBytes;
      while ( fnCompare( pPivot, pHigh) > -1 && pLow < pHigh ) pHigh -= p_nSizeBytes;
      
      if( pLow < pHigh ) 
         Swap( pLow, pHigh, pTmp, p_nSizeBytes );
   }
   // In this point: pLow == pHigh
   mem_copy_c(p_pLast, pHigh, p_nSizeBytes);
   mem_copy_c(pHigh, pPivot, p_nSizeBytes);
   
   mem_delete(pTmp);
   mem_delete(pPivot);
   // ------ End Partition ------
   
   QSort_Desc_c( p_pFirst,        pLow-p_nSizeBytes, p_nSizeBytes, fnCompare );
   QSort_Desc_c( pHigh+p_nSizeBytes,  p_pLast,       p_nSizeBytes, fnCompare );
}

//--------------------------------------------------------------------
