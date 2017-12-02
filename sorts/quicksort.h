/***************************************************************************************
 * @Algorithm :  Quick Sort.
 *               Simple adaptation to 'C99' or 'C++98' from:
 *                 QSortAlgorithm.java	1.6f 95/01/31 James Gosling
 *                 Copyright (c) 1994-1995 Sun Microsystems, Inc. All Rights Reserved.
 *                 @author       : James Gosling
 *                 @version      : 1.6f, 31 Jan 1995
 *                 @last modified: 09 Jan 1998
 *
 * @Adapted by: Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, Perú.
 ***************************************************************************************/


//-------------------------------------------------------------------------

void QuickSort_Asc( long* p_Array, int first, int last) ;
void QuickSort_Desc(long* p_Array, int first, int last) ;

#define QuickSort(   _Array, _nLength, ...) csname2(QuickSort_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define QuickSort_0( _Array, _nLength)                        _QuickSort( _Array, _nLength, ASC )
#define QuickSort_1( _Array, _nLength, _bIsAscending)         _QuickSort( _Array, _nLength, (_bIsAscending) )
#define QuickSort_2( _Array, _nLength, _Type, _bIsAscending)  _QuickSort( _Array, _nLength, (_bIsAscending) )

inline void _QuickSort(long* p_Array, size_t nLength, bool p_bAscending ) {
   if (nLength < 2) return;
   if( p_bAscending )
      QuickSort_Asc(p_Array, 0, nLength-1);
   else
      QuickSort_Desc(p_Array, 0, nLength-1);
}

// ...............................................

void QuickSort_Asc(long* p_Array, int p_nFirst, int p_nLast) {
   if (p_nFirst >= p_nLast)  return;

   long vTmp;

   if( p_nFirst == (p_nLast - 1) ) {
      if (p_Array[p_nFirst] > p_Array[p_nLast])
         Swap( p_Array[p_nFirst], p_Array[p_nLast], vTmp );
      return;
   }

   // ------ Begin Partition ------
   int  nLow  = p_nFirst,
        nHigh = p_nLast,
        nPivot ;
   long vPivot ;

   // Same as (p_nFirst+p_nLast)/2, but avoids overflow for large p_nFirst and p_nLast.
   Swap( p_Array[ p_nFirst+(p_nLast-p_nFirst)/2],  p_Array[p_nLast],  vPivot );

   while ( nLow < nHigh ) {
      while ( p_Array[nLow] <= vPivot  && nLow < nHigh ) nLow++;
      while ( vPivot <= p_Array[nHigh] && nLow < nHigh ) nHigh--;

      if( nLow < nHigh )
         Swap( p_Array[nLow],  p_Array[nHigh],  vTmp );
   }

   p_Array[p_nLast] = p_Array[nHigh];
   p_Array[nHigh]   = vPivot;

   // In this point: nLow == nHigh
   nPivot = nLow ;
   // ------ End Partition ------

   QuickSort_Asc(p_Array, p_nFirst,   nPivot-1);
   QuickSort_Asc(p_Array, nPivot+1, p_nLast   );
}

// ...............................................

void QuickSort_Desc(long* p_Array, int p_nFirst, int p_nLast) {
   if (p_nFirst >= p_nLast)  return;

   long vTmp;

   if( p_nFirst == (p_nLast - 1) ) {
      if (p_Array[p_nFirst] < p_Array[p_nLast])
         Swap( p_Array[p_nFirst], p_Array[p_nLast], vTmp );
      return;
   }

   // ------ Begin Partition ------
   long vPivot;

   int nPivot,
       nLow = p_nFirst,
       nHigh = p_nLast;

   // Same as (p_nFirst+p_nLast)/2, but avoids overflow for
   // large p_nFirst and p_nLast.
   Swap( p_Array[ p_nFirst+(p_nLast-p_nFirst)/2],  p_Array[p_nLast],  vPivot );

   while( nLow < nHigh ) {
      while( nLow < nHigh && p_Array[nLow] >= vPivot  ) ++nLow;
      while( nLow < nHigh && vPivot >= p_Array[nHigh] ) --nHigh;

      if( nLow < nHigh )
         Swap( p_Array[nLow], p_Array[nHigh], vTmp );
   }

   p_Array[p_nLast] = p_Array[nHigh];
   p_Array[nHigh] = vPivot;

   // In this point: nLow == nHigh
   nPivot = nLow;
   // ------ End Partition ------

   QuickSort_Desc(p_Array, p_nFirst,   nPivot-1);
   QuickSort_Desc(p_Array, nPivot+1, p_nLast   );
}

//--------------------------------------------------------------------
