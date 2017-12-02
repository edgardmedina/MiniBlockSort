/*********************************************************************************************
 * @Algorith   : Flashsort
 *               Generic adaptation in 'C' pure, to works in 'C' and/or C++ from:
 *                 Source        : Flashsort.f95	
 *                 @author       : Karl-Dietrich Neubert
 *                 @version      : Copyright (c) K. - D. NEUBERT  1997.
 *                 @last modified: 09 Jan 1998
 * 
 * @Adapted by : Edgard Medina Q. <edgardmedina@yahoo.com>
 * 
 * @Observation: This algorith is generic and minimun requiere ISO C99 or C++98,
 *               suport native types or custom C/C++ Structures (variables) or C++ Classes (objects).
 * 
 *               To work correctly is necessary define a Funtor compare with prototype:
 *                   int (*fnCompare)(const void*, const void*);
 * 
 *               NOTE: in C++, the funtor must be declared as 'friend function' to Class.
 * 
 *********************************************************************************************/
// ----------------------------------------------------------------------------
void FlashSort_Asc( long*,int);

// #define FlashSort(   _Array, _nLength, ...) csname2(FlashSort_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
// #define FlashSort_0( _Array, _nLength)                     
// #define FlashSort_1( _Array, _nLength, _bAscending)        
// #define FlashSort_2( _Array, _nLength, _Type, _bAscending) 

#define FlashSort(   _Array, _nLength, ...) csname2(FlashSort_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define FlashSort_0( _Array, _nLength)                        _FlashSort( _Array, _nLength, ASC )
#define FlashSort_1( _Array, _nLength, _bIsAscending)         _FlashSort( _Array, _nLength, (_bIsAscending) )
#define FlashSort_2( _Array, _nLength, _Type, _bIsAscending)  _FlashSort( _Array, _nLength, (_bIsAscending) )


inline void _FlashSort(long* Array, size_t nLength, bool p_bAscending ) {
   if (nLength < 2) return;
   if( p_bAscending )
      FlashSort_Asc(Array, nLength);
   else
      FlashSort_Asc(Array, nLength);
}


void FlashSort_Asc(   // Procedure: Sort an Array
     long* A      //     Param: Array to sort
    ,int   nA     //     Param: Size of Array.
) {               //   Returns: Array 'A' sorted.
   // Sorts array A with nA elements by use of index vector L 
   // of dimension nL with nL about 0.1 nA. 
   // Copyright (C)  k. - D. Neubert  1997                           
   if( nA < 3 ) {
      fprintf(stderr, "Error FlashSort: The array size must be greater than 2 \n");
      return;
   }
   int i = 0, j = 0, k = 0, nL;
   long vHold , 
        vFlash ; 
   if(      nA < 20  ) nL = 3 ;
   else if( nA < 40  ) nL = 4 ;
   else if( nA < 60  ) nL = 5 ;
   else if( nA < 100 ) nL = 10 ;
   else if( nA < 150 ) nL = 15 ;
   else   nL = 20 ;
   nL = nA / nL ;
   //============================ CLASS FORMATION ===== 
   // k(A(i)) = 1 + INT( (nL-1) (A(i)-Amin) / (Amax-Amin) )
   long vMin=A[0];
   int  iMax = 0 ;
   
   for( i = 1 ; i < nA ; ++i ) {
      if( A[i] < vMin )    vMin = A[i] ;
      if( A[i] > A[iMax] ) iMax = i ;
   }
   
   if( vMin == A[iMax] ) return;

   double c1 = ((double)nL - 1.0) / (A[iMax] - vMin);
   int*   L  = (int*)calloc(nL, sizeof(int) ) ; // Important: use calloc(), not malloc().
   
   for( i=0; i < nA; ++i ) {
      k =  (int)( c1 * (A[i] - vMin) );
      ++L[k] ;
   }
   
   for( k = 1 ; k < nL ; ++k ){
      L[k] += L[k - 1];
   }
   
   Swap( A[0], A[iMax], vHold);
   
   //=============================== PERMUTATION =====
   int nPos;
   int nMove = 0 ;
   j = 0 ;
   k = nL - 1 ;
   while( nMove < nA - 1 ) {
      while( j > L[k] - 1 ) {
         ++j ;
         k = (int)( c1 * (A[j] - vMin) ) ;
      }
      
      vFlash = A[j] ;
      
      while( j != L[k] ) {
         k = (int)(c1 * (vFlash - vMin)) ;
         nPos = L[k] - 1;
         Swap(vFlash, A[nPos], vHold );
         
         --L[k]  ;
         ++nMove ;
      }
   }
   free( L ) ;
   //========================= STRAIGHT INSERTION =====
   for( i = (nA-1)-2 ; i > -1 ; --i ){
      if( A[i + 1] < A[i] ){
         vHold = A[i] ;
         j = i ;
         while( A[j + 1] < vHold ){
            A[j] = A[j + 1] ; 
            ++j ;
         }
         A[j] = vHold ;
      }
   }
   //=========================== END FLASH1 =====
}

//--------------------------------------------------------------------
