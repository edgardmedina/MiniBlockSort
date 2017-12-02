/***************************************************************************************
 * @Algorithm :  Insert Sort.
 *               Simple adaptation to 'C99' or 'C++98' from:
 *
 * @Adapted by: Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, Perú.
 ***************************************************************************************/

//-------------------------------------------------------------------------
void InsertSort_Asc(long* Array, int nFirst, int nLast) ;
void InsertSort_Desc(long* Array, int nFirst, int nLast) ;

#define InsertSort( _Array, nLength, ...) csname2(InsertSort_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define InsertSort_0( Array, nLength)                        _InsertSort( (Array), (nLength), ASC )
#define InsertSort_1( Array, nLength, _bIsAscending)         _InsertSort( (Array), (nLength), (_bIsAscending) )
#define InsertSort_2( Array, nLength, _Type, _bIsAscending)  _InsertSort( (Array), (nLength), (_bIsAscending) )

inline void _InsertSort(long* Array, size_t nLength, bool p_bAscending ) {
   if (nLength < 2) return;
   if( p_bAscending )
      InsertSort_Asc(Array, 0, nLength-1);
   else
      InsertSort_Desc(Array, 0, nLength-1);
}

// ...............................................

void InsertSort_Asc(long* Array, int nFirst, int nLast) {
   int nStop = nLast +1;
   long vTemp;
   for (int i = nFirst + 1; i < nStop; i++) 
      for (int j = i; nFirst < j  &&  Array[j - 1] > Array[j] ; --j) 
         Swap( Array[j - 1], Array[j], vTemp );
}

// ...............................................

void InsertSort_Desc(long* Array, int nFirst, int nLast) {
   int nStop = nLast +1;
   long vTemp;
   for (int i = nFirst + 1; i < nStop; i++) 
      for (int j = i; nFirst < j  && Array[j] > Array[j - 1]; --j) 
         Swap( Array[j - 1], Array[j], vTemp );
}

//--------------------------------------------------------------------
