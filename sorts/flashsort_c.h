/*********************************************************************************************
 * @Algorith   : Flashsort
 *               Generic adaptation in 'C' pure, to works in 'C' and/or C++ from:
 *                 Source        : Flashsort.f95
 *                 @author       : Karl-Dietrich Neubert
 *                 @version      : Copyright (c) K. - D. NEUBERT  1997.
 *                 @last modified: 09 Jan 1998
 *
 * @Adapted by : Edgard Medina Q. <edgardmedina@yahoo.com>, Arequipa, Perú
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
#ifdef __cplusplus
   #include <cstdio>
   #include <cstdlib>
   #include <cfloat>
   #include <cstring>
#else
   #include <stdio.h>
   #include <stdlib.h>
   #include <float.h>
   #include <string.h>
#endif

// ----------------------------------------------------------------------------

typedef char* str;
typedef unsigned char byte;
typedef int (*fnCompare_t)(const void*, const void*);
typedef double (*fnDifference_t)(const void*, const void*);

//  Set allocated memory in raw format.
#define mem_new(  _Type, _nSlots ) (_Type*)malloc( (_nSlots) * sizeof(_Type) )

//  Set allocated memory initialized with zeros.
#define mem_newz( _Type, _nSlots ) (_Type*)calloc( (_nSlots),  sizeof(_Type) )

#define mem_free(_pVar) free(_pVar)

// Copy a memory block to another position
void mem_copy_c( void* p_pTarget, const void* p_pSource, size_t p_nBytes ) {
   byte* pTarget = (byte*)p_pTarget;
   byte* pSource = (byte*)p_pSource;
   while( p_nBytes-- ) *(pTarget++) = *(pSource++);
}

void swap3_c( void* p_Ptr1, void* p_Ptr2, size_t p_nBytes, void* p_PtrTmp ) {
   mem_copy_c( p_PtrTmp, p_Ptr1,   p_nBytes );
   mem_copy_c( p_Ptr1,   p_Ptr2,   p_nBytes );
   mem_copy_c( p_Ptr2,   p_PtrTmp, p_nBytes );
}

//-------------------------[ Here define some Funtors Compare ]--------------------------------------

#define Max2( a, b ) ( (a)>(b) ? (a) : (b) )

#define FNCOMPARE( _Type )  Compare_ ## _Type

// #define  str_Compare strcmp
inline int Compare_str(const void* p_ptr1, const void* p_ptr2) {
   printf("\t%\"%s\" : \"%s\"\n", *(char**)p_ptr1, *(char**)p_ptr2 );
   return (int)strcmp( *(char**)p_ptr1, *(char**)p_ptr2);
}

inline int Compare_short(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(short*)p_ptr1 - *(short*)p_ptr2);
}

inline int Compare_long(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(long*)p_ptr1 - *(long*)p_ptr2);
}

inline int Compare_int(const void* p_ptr1, const void* p_ptr2) {
   return *(int*)p_ptr1 - *(int*)p_ptr2;
}

inline int Compare_double(const void* p_ptr1, const void* p_ptr2) {
    double nA = *(double*)p_ptr1,
           nB = *(double*)p_ptr2,
           nDiff = nA - nB ;
    nA = fabs(nA);
    nB = fabs(nB);
    return  fabs( nDiff ) <= Max2(nA,nB)*DBL_EPSILON ? 0 : (nDiff<0.0 ? -1 : 1) ;
}

//--------------------------------------------------------------------
#define FNDIFFERENCES( _Type )  _Type ## _Difference

inline double Difference_str(const void* p_ptr1, const void* p_ptr2) {
   return (double)strcmp( *(char**)p_ptr1, *(char**)p_ptr2);
}

inline double Difference_short(const void* p_ptr1, const void* p_ptr2) {
   return (double)(*(short*)p_ptr1 - *(short*)p_ptr2);
}

inline double Difference_long(const void* p_ptr1, const void* p_ptr2) {
   return (long)(*(long*)p_ptr1 - *(long*)p_ptr2);
}

inline double Difference_int(const void* p_ptr1, const void* p_ptr2) {
   return (double)(*(int*)p_ptr1 - *(int*)p_ptr2);
}

inline double Difference_double(const void* p_ptr1, const void* p_ptr2) {
    return *(double*)p_ptr1- *(double*)p_ptr2 ;
}

// ----------------------------------------------------------------------------

#define Flashsort( _Array, _nSize, _Type) \
         FlashSort_c( (byte*)_Array, (int)_nSize, sizeof(*(_Array)), FNCOMPARE( _Type ), FNDIFFERENCES( _Type ) )

void FlashSort_c(             // Procedure: Sort an Array
     byte*          A         //     Param: Array to sort
    ,int            nA        //     Param: Size of Array.
    ,size_t         nBytes    //     Param: Size of each slot in array.
    ,fnCompare_t    fnCompare //     Param: Funtor to compare two tokens.
    ,fnDifference_t fnDiff    //     Param: Function to make a diference between 2 tokens.
) {                           //   Returns: Array 'A' sorted.
   // Sorts array A with nA elements by use of index vector L
   // of dimension nL with nL about 0.1 nA.
   // Copyright (C)  k. - D. Neubert  1997
   if( nA < 3 ) {
      fprintf(stderr, "Error FlashSort: The array size must be greater than 2 \n");
      return;
   }
   int i = 0, j = 0, k = 0, nL;
   if(      nA < 20  ) nL = 3 ;
   else if( nA < 40  ) nL = 4 ;
   else if( nA < 60  ) nL = 5 ;
   else if( nA < 100 ) nL = 10 ;
   else if( nA < 150 ) nL = 15 ;
   else   nL = 20 ;
   nL = nA / nL ;
   //============================ CLASS FORMATION =====
   // k(A(i)) = 1 + INT( (nL-1) (A(i)-Amin) / (Amax-Amin) )
   byte* pvMin= mem_new( byte, nBytes );
   mem_copy_c(pvMin, A, nBytes );
   byte *pMax = A;
   byte *iterI,
        *pStop = A + (nA*nBytes) ;

   for( iterI = A + nBytes ; iterI < pStop ; iterI += nBytes  ) {
      if( fnCompare( iterI, pvMin ) < 0 ) mem_copy_c( pvMin,  iterI, nBytes) ;
      if( fnCompare( iterI, pMax  ) > 0 ) pMax = iterI ;
   }

   if( fnCompare( pvMin, pMax ) == 0 ) {
      mem_free( pvMin );
      return;
   }

   byte *pvHold  = mem_new( byte, nBytes ) ,
        *pvFlash = mem_new( byte, nBytes ) ;

   double c1 = ((double)nL - 1.0) / fnDiff( pMax, pvMin);
   int*   L  = mem_newz( int, nL ) ;      // Important: use mem_newz(), not mem_new().

   for( iterI = A ; iterI < pStop ; iterI += nBytes ) {
      k =  (int)( c1 * fnDiff( iterI, pvMin) );
      ++L[k] ;
   }

   for( k = 1 ; k < nL ; ++k )
      L[k] += L[k - 1];

   swap3_c( A, pMax, nBytes, pvHold ) ;

   //=============================== PERMUTATION =====
   int nMove = 0 ;
   j = 0 ;
   k = nL - 1 ;
   iterI = A ;

   while( nMove < nA - 1 ) {
      while( j > L[k] - 1 ) {
         ++j ; iterI += nBytes ;
         k = (int)( c1 * fnDiff( iterI, pvMin) ) ;
      }

      mem_copy_c( pvFlash, iterI, nBytes ) ;

      while( j != L[k] ) {
         k = (int)(c1 * fnDiff( pvFlash, pvMin)) ;

         swap3_c( pvFlash, A+( (L[k] - 1)*nBytes), nBytes, pvHold) ;

         --L[k]  ;
         ++nMove ;
      }
   }
   mem_free( L ) ;
   mem_free( pvMin );

   // //========================= STRAIGHT INSERTION =====
   void *iterNext ,
        *iterJ ;
   pStop = A - 1 ;
   for( iterI = A + ((nA-1)-2)*nBytes ; iterI > pStop ; iterI -= nBytes ) {
      iterNext = iterI + nBytes ;
      if( fnCompare( iterNext,  iterI ) < 0 ) {
         mem_copy_c( pvHold, iterI, nBytes ) ;
         iterJ = iterI ;
         while( fnCompare( iterNext, pvHold) < 0 ) {
            mem_copy_c( iterJ, iterNext, nBytes ) ;
            iterJ += nBytes ;
            iterNext += nBytes ;
         }
         mem_copy_c( iterJ, pvHold, nBytes) ;
      }
   }
   //=========================== END FLASH1 =====
   mem_free( pvHold  );
   mem_free( pvFlash );
}

//--------------------------------------------------------------------

#define Procedure    do{
#define EndProcedure }while(0)

#define show_array( _Type, _Mask, _Array, _Size ) \
   Procedure\
      if( !(_Size) ) return;\
      _Type *iterI= (_Type*)(_Array),\
            *pStop= (_Type*)(_Array) + (_Size) ;\
      printf("\n Array(%d) = { " _Mask, (_Size), *iterI );\
      for( ++iterI ; iterI < pStop ; ++iterI ) printf(", " _Mask, *iterI);\
      printf(" }\n");\
   EndProcedure

#define sizeofarray( array ) (sizeof(array)/sizeof(*array))

//--------------------------------------------------------------------

int main(int nArg, char** Args) {
   int nArr;

   long arr1[] = {42, 26, 16, 10, 21, 63, 3, 46, 41, 7, 5 };
   nArr = sizeofarray(arr1);

   printf("\n-------------------[ Sort Ascending (long) Generic ]-------------------\n");

   show_array( long, "%d", arr1, nArr );
   Flashsort( arr1, nArr, long ) ;
   show_array( long, "%d", arr1, nArr );

   printf("\n-------------------[ Sort Ascending (double) Generic ]-------------------\n");

   double arr2[] = {42., 26., 16., 10., 21., 63., 3., 46., 41., 7., 5. };
   nArr = sizeofarray(arr2);

   show_array( double, "%3.1f", arr2, nArr );
   Flashsort( arr2, nArr, double ) ;
   show_array( double, "%3.1f", arr2, nArr );

   printf("\n-------------------[ Sort Ascending (string) Generic ]-------------------\n");

   char* arr3[] = { "sould", "mail", "road", "rail", "again", "toad", "mall", "liceo", "dice", "join" } ;
   nArr = sizeofarray(arr3);

   show_array( str, "%s", arr3, nArr );
   Flashsort( arr3, nArr, str ) ;
   show_array( double, "%s", arr3, nArr );

   printf("\n---------------------------------------------------------\n");

}


