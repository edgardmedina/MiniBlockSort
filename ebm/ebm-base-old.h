/********************************************************************************************************
 * @Objetive : Provide generic functions to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             functions to manipulate memroy load the standard header for
 * @version  : v1.0, Copyleft 2010
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 ********************************************************************************************************
 *  Constants:
 *  ---------
 *
 *  const int  ASC=true;
 *  const int  DESC=false;
 *
 *
 * Miscellaneous Template Functions :
 * --------------------------------
 *  <Type> Max2( <Type> a, <Type> b  )  : Get maximun value of 'a' or 'b' number values,
 *
 *  void Swap( <Type> a, <Type> b [, <Type> tmp] )  : Swapped two values, optionally let to use a
 *                                                    a extern temporal variable for best performance
 *                                                    in loops.
 * void Randomize( [size_t Init_Value] )   : Initialize the seed for Random() functions.
 *                                              Init_Value:  {empty}  Generate a random value from system clock.
 *                                                            n < 0   Generate a random value from system clock.
 *                                                            0 <= n  Generate a random value from 'n' base number.
 *
 * int  Random( [size_t nTop_Value] )      : Generate random number. optionally you can specified a top value for random values.
 *
 *
 * Memory Manipulation:
 * -------------------
 *  * Functions to copy the values from Source to Target:
 *     <Type>* mem_copy(   <Type> Target, <Type>      Source )                 // Template function.
 *     void*   mem_copy_c( void* pTarget, const void* pSource, size_t nBytes ) // Pure 'C' style
 *
 *  * Template Functions to allocate memory for a single object
 *      <Type>* mem_new( {                           //  Template Function to allocate memory a single object.
 *                            <Type>}                //  Explicit Type (ISO C/C++ or custom type)
 *                         [, <Type> _Init_Value]    //  (Optional) Initial value to set in Allocated memory. NOTE: This argument must refernce to LVALUE variable.
 *                       )
 *      void    mem_delete( <Type>* ptr_Var )        // Template Function to delete a allocated memory by mem_new().
 *
 *  * Template Functions to allocate memory for an Array of objects
 *      <Type>* array_new(                           //  Template Function to allocate memory a single object.
 *                         {<Type>}                  //  Explicit Type (ISO C/C++ or custom type)
 *                        ,size_t  nSlots            //  Quantity of slots in array.
 *                       [,<Type>  Init_Value]       //  (Optional) Initial value to set in Allocated memory. NOTE: This argument must refernce to LVALUE variable.
 *                       )
 *      void    array_delete( <Type>* ptr_Array )
 *
 *  * Pure 'C' Functions to allocate memory for an object or Array of objects
 *     void* mem_alloc(  size_t p_nSlots, size_t p_nSizeBytes ) // Allocate memory for one or many objects of size 'p_nSizeBytes'
 *     void* mem_allocz( size_t p_nSlots, size_t p_nSizeBytes ) // Idem 'mem_alloc()', but initialize the memory with zeros values.
 *     void  mem_free(_pVar) free(_pVar)                          // Release memory allocated by 'mem_alloc()' or 'mem_allocz()'.
 *
 *
 * Array Functions:
 * ----------------
 *  * Check out if array ir sorted in ascendig mode or descending mode.
 *     void  Check_Sorted( long* Array, int nSlotsArray, bool isAscending) :
 *
 *  * Template procedure to show an array on screen
 *     void  show_array( <Type>, const char* strMask, <type>* Array, size_t nSlotsArray )
 *
 *  * Function get quantity of slots in Static Array. NOTE: don't work in Dynamic Arrays.
 *     int sizeofarray( Array )
 *
 * ------------------------------------------------------------------------------------
 *
 *  Private Macros To Create Function Templates in 'C'
 * ---------------------------------------------------
 *
 *  This macros let me create a limited style of overload of functions to agregate most power
 *  to C programing
 *
 *  * Template Procedure:
 *        #define  <MacroName>( <Argument_list> )\
 *            Procedure\
 *               ... \   // use here the arguments
 *            EndProcedure
 *
 *  * Create a C Command:
 *        csname2(  token1, token2 )  : joint two tokens to form a single name of command (macro, function, variable or whatever).
 *
 *  * Macro to count quantity of Variadic Argument. Support from 0 until 62 arguments, it returns a LITERAL Number
 *       size_t Count_Args( __VA_ARGS__ )
 *
 ********************************************************************************************************/
// #define TEMPLATE


// With this include header is not necessary add 'extern C {' and '}' at beggining and end of this file
// and another header files.
#ifdef __cplusplus
   #include <cstdlib>
   #include <cstdio>
   #include <ctime>
   #include <cfloat>
   #include <cstring>
   #include <cmath>
#else
   #include <stdlib.h>
   #include <stdio.h>
   #include <stdbool.h>
   #include <time.h>
   #include <float.h>
   #include <string.h>
   #include <math.h>
#endif
#if defined(_WIN32) && !defined(_WINDOWS_)
   #include <windows.h> // for funtion time
#elif !defined(_WIN32)
   #include <sys/unistd.h>
#endif
#include <sys/time.h>
#include <sys/timeb.h>

// for use C template functions for basic types.. The USER if want use this, then he
// must use '#define C_TEMPLATE' before '#include "ebm_base.h"'

// --------------------[ Defining Global resources ]--------------------
const int  ASC=true;
const int  DESC=false;

typedef int (*fnCompare_t)(const void*, const void*);

typedef unsigned char          Byte;
typedef unsigned char          byte, uchar;
typedef unsigned short         ushort;
typedef unsigned int           uint;
typedef unsigned long          ulong;
typedef long long int          llong;
typedef unsigned long long int ullong;
typedef long double            ldouble;

#define and     &&
#define and_eq  &=
#define bitand  &
#define bitor   |
#define compl   ~
#define not     !
#define not_eq  !=
#define or      ||
#define or_eq   |=
#define xor     ^
#define xor_eq  ^=

#ifdef __cplusplus
    extern "C" {
#endif

/* Define a loop structure 'Foreach()'
 * Syntax:   Foreachin( <datatype>, <var_Iterator>, <value1> [, <value2>, ... , <valueN>] ) Do
 *              // ...   So something
 *           EndFor
 *******/ 
#define Foreach( _Type, _Iter, _Item, ... ) \
      for( _Type _arrList[] = {_Item, ##__VA_ARGS__}, \
                *_InIter = _arrList, \
                *_Stop   = _InIter + sizeofarray(_arrList), \
                 _Iter   = *_InIter \
          ; _InIter < _Stop \
          ; _Iter = *(++_InIter) ) 

// --------------------[ Private macros for C Templates ]--------------------

#define Procedure    do{
#define EndProcedure }while(0)


#define _STR( _Token ) #_Token
#define STR( _Token ) _STR( _Token )

//---------------------------------------------
#define csname2_( _a, _b )                 _a ## _b
#define csname2(  _a, _b )         csname2_( _a, _b )
#define csname3_( _a, _b, _c )             _a ## _b ## _c
#define csname3(  _a, _b, _c )     csname3_( _a, _b, _c )

// #define csname2_( _arg, ...) _arg ## __VA_ARGS__
// #define csname2(  _arg, ...) csname2_(_arg, __VA_ARGS__)
// #define csname3_( _arg, _arg2, ...) _arg ## _arg2 ## __VA_ARGS__
// #define csname3(  _arg, _arg2, ...) csname3_(_arg, _arg2, __VA_ARGS__)
//---------------------------------------------

// Macro to count quantity of Variadic Argument. Support from 0 until 62 arguments
// more info in thread:  <https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s>
#define Count_Args(...)            __Expand_Count_Args_(_0,  ##__VA_ARGS__, __ReverseSequence_N() )
#define __Expand_Count_Args_(...)  __Aux_Count_N_Args_(__VA_ARGS__)
#define __Aux_Count_N_Args_( \
        __1,  __2,  __3,  __4,  __5,  __6,  __7,  __8,  __9,  __10, \
        __11, __12, __13, __14, __15, __16, __17, __18, __19, __20, \
        __21, __22, __23, __24, __25, __26, __27, __28, __29, __30, \
        __31, __32, __33, __34, __35, __36, __37, __38, __39, __40, \
        __41, __42, __43, __44, __45, __46, __47, __48, __49, __50, \
        __51, __52, __53, __54, __55, __56, __57, __58, __59, __60, \
        __61, __62, __63,   \
        N, ...) N
#define __ReverseSequence_N() \
        62, 61, 60,                             \
        59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
        49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
        29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
        19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
         9,  8,  7,  6,  5,  4,  3,  2,  1,  0

// --------------------[ Public macros and methods ]--------------------

#ifdef __cplusplus
    #define Record( _Name )  struct _Name
    #define Struct( _Name )  struct _Name
    #define Union(  _Name )  union  _Name
    #define Enum(   _Name )  enum   _Name
#else
    #define _ebm_C_Structure( _Tstruct, _Name ) \
        _Tstruct csname3(  _ebm_, _Tstruct, _Name );\
        typedef _Tstruct csname3(  _ebm_, _Tstruct, _Name ) _Name;\
        _Tstruct csname3(  _ebm_, _Tstruct, _Name )
    #define Record( _Name ) _ebm_C_Structure( struct, _Name )
    #define Struct( _Name ) _ebm_C_Structure( struct, _Name )
    #define Union(  _Name ) _ebm_C_Structure( union,  _Name )
    #define Enum(   _Name ) _ebm_C_Structure( enum,   _Name )
#endif

#define Max2( _a, _b ) ( (_a)>(_b) ? (_a) : (_b) )
#define Min2( _a, _b ) ( (_a)<(_b) ? (_a) : (_b) )

// ------------------[ Memory Manager ]------------------

inline void  mem_copy_c( register void* p_pTarget, register const void* p_pSource, register size_t p_nBytes ) {
   if( !p_pTarget ) return NULL;
    register byte *pTarget = (byte *)p_pTarget;
    register byte *pSource = (byte *)p_pSource;
    while( p_nBytes-- ) *pTarget++ = *pSource++;
}

#define mem_copy( _Target, _Source)   mem_copy_c( (void*)&(_Target), (void*)&(_Source), (size_t)sizeof(_Target) )

void* _gTmpVar_;  // Global variable for general propose

#ifdef __cplusplus
   #define array_new( _Type, _nSlots, ...) csname2(_Array_New_, Count_Args(__VA_ARGS__))( _Type, _nSlots, ##__VA_ARGS__ )
   #define _Array_New_0( _Type, _nSlots)               new _Type [ (_nSlots)<2 ? 1 : (size_t)(_nSlots) ]
   #define _Array_New_1( _Type, _nSlots, _InitValue)   new _Type [ (_nSlots)<2 ? 1 : (size_t)(_nSlots) ] ((_InitValue))
   #define array_delete( _Ptr_Array ) if( (_Ptr_Array) ) { delete[] (_Ptr_Array); (_Ptr_Array) = NULL; }

   #define mem_new( _Type, ...)    csname2(_Mem_New_, Count_Args(__VA_ARGS__))( _Type, ##__VA_ARGS__ )
   #define _Mem_New_0( _Type )         new _Type
   #define _Mem_New_1( _Type, _nValue) new _Type ((_nValue))

   #define mem_delete( _Ptr_Var ) if( (_Ptr_Var) ) { delete (_Ptr_Var); (_Ptr_Var) = NULL; }
#else
   #define array_new( _Type, _nSlots, ...) csname2(_Array_New_, Count_Args(__VA_ARGS__))( _Type, _nSlots, ##__VA_ARGS__ )
   #define _Array_New_0( _Type, _nSlots)               (_Type*)malloc( ( (_nSlots)<2 ? 1 :(size_t)(_nSlots) ) * sizeof(_Type) )
   #define _Array_New_1( _Type, _nSlots, _InitValue)   (_Type*)Array_New_Init( (_nSlots), sizeof(_Type), (void*)&(_InitValue) )
   
   void* Array_New_Init( size_t nSlots, size_t nSizeBytes, void* pInitValue)  {
      nSlots = (nSlots)<2 ? 1 : (size_t)(nSlots)  ;
      byte *pAlloc= (byte*) malloc( nSlots * nSizeBytes ),
           *iter = pAlloc,
           *stop = pAlloc +  nSlots ;
      while( iter < stop ) mem_copy_c( iter++, pInitValue, nSizeBytes ) ;
      return (void*)pAlloc;
   }
   
   #define array_delete( _Ptr_Array ) if( (_Ptr_Array) ) { free(_Ptr_Array); (_Ptr_Array) = NULL; }

   #define mem_new( _Type,  ... )  csname2( _Mem_New_, Count_Args(__VA_ARGS__))( _Type, ##__VA_ARGS__)
   #define _Mem_New_0(_Type)  (_Type*)malloc(sizeof(_Type))
   #define _Mem_New_1(_Type, _InitValue)  (_Type*)mem_copy_c( malloc( sizeof(_Type) ), (void*)&(_InitValue), sizeof(_Type) )
   #define mem_delete(_pVar) if( (_pVar) ) free(_pVar)
#endif

#define matrix_new( \
        _Type     /* C or C++ Datatype or custom datatype. */ \
      , _IO_var   /* Variable to set memory of matrix. */ \
      , _nRows    /* Rows in matrix. */ \
      , _nCols    /* Columns in matrix. */ \
      , ...       /* (Optional) Value to set in matrix */ \
   ) \
   Procedure \
      _IO_var = array_new(_Type*, _nRows) ;\
      register _Type **iter=_IO_var ; \
      if( _IO_var ) \
         for( int i=0 ; i < _nRows ; ++i ) \
            *(iter++) = array_new( _Type, _nCols, ##__VA_ARGS__ ) ; \
   EndProcedure
   
// #define matrix_delete( _IOvar, _nRows) 
#define matrix_delete( _IOvar, _nRows, ...)  csname2(_Matrix_Delete_,Count_Args(__VA_ARGS__))(_IOvar, _nRows, ##__VA_ARGS__ )

#define _Matrix_Delete_0( _IOvar, _nRows ) \
   Procedure\
      size_t nRows = (size_t)(_nRows) ; \
      for(size_t i=0 ; i < nRows ; ++i ) \
         array_delete( _IOvar[i] ) ; \
      array_delete( _IOvar ) ; \
   EndProcedure
   
#define _Matrix_Delete_1( _IOvar, _nRows, _nCols )  \
   Procedure\
      size_t nRows = (size_t)(_nRows) , \
             nCols = (size_t)(_nCols) ; \
      for(size_t i=0 ; i < nRows ; ++i ) { \
         for(size_t j=0 ; j < nCols ; ++j ) \
            mem_delete( _IOvar[i][j] ) ; \
         array_delete( _IOvar[i] ) ; \
      } \
      array_delete( _IOvar ) ; \
   EndProcedure

void* mem_alloc( size_t p_nSlots, size_t p_nSizeBytes ) ;
void* mem_allocz( size_t p_nSlots, size_t p_nSizeBytes ) ;
inline void  mem_copy_c( void* p_pTarget, const void* p_pSource, size_t p_nBytes ) ;


void* mem_alloc( size_t p_nSlots, size_t p_nSizeBytes ) {
   return malloc( ( p_nSlots<2 ? 1 : p_nSlots)  * p_nSizeBytes ) ;
}
void* mem_allocz( size_t p_nSlots, size_t p_nSizeBytes ) {
   return calloc( ( p_nSlots<2 ? 1 : p_nSlots), p_nSizeBytes ) ;
}
#define mem_free(_pVar) if(_pVar) { free(_pVar); _pVar = NULL; }
// ------------------[ End Memory Manager ]------------------

// ------------------------------------------------------------------
#ifdef TEMPLATE
    #define  swap_c( _Ptr1, _Ptr2, _nBytes ) \
       Procedure\
          register Byte   _cTmp;\
          register Byte *_ptr1 = (Byte *)_Ptr1;\
          register Byte *_ptr2 = (Byte *)_Ptr2;\
          register size_t _nSBytes = _nBytes ;\
          do {\
                _cTmp = *_ptr1 ,\
             *_ptr1++ = *_ptr2 ,\
             *_ptr2++ = _cTmp ;\
          } while( --_nSBytes > 0 );\
       EndProcedure
#else
    inline void  swap_c(  register Byte* p_Ptr1
                        , register Byte* p_Ptr2
                        , register size_t p_nBytes 
    ) {
       register Byte   cTmp;
       while( p_nBytes-- )
               cTmp = *p_Ptr1,
          *p_Ptr1++ = *p_Ptr2,
          *p_Ptr2++ = cTmp ;
    }
#endif

#ifdef TEMPLATE
    #define swap3_c( _Ptr1,  _Ptr2, _PtrTmp, _nBytes) \
       Procedure\
          register Byte *_pTmp = (Byte *)_PtrTmp;\
          register Byte *_ptr1 = (Byte *)_Ptr1;\
          register Byte *_ptr2 = (Byte *)_Ptr2;\
          register size_t _nSBytes = (_nBytes) ;\
          do {\
             *_pTmp   = *_ptr1 ,\
             *_ptr1++ = *_ptr2 ,\
             *_ptr2++ = *_pTmp++ ;\
          } while( --_nSBytes > 0 );\
       EndProcedure
#else
    inline void  swap3_c(  register Byte* p_Ptr1
                         , register Byte* p_Ptr2
                         , register Byte* p_PtrTmp
                         , register size_t p_nBytes
    ) {
       while( p_nBytes-- )
          *p_PtrTmp = *p_Ptr1,
          *p_Ptr1++ = *p_Ptr2,
          *p_Ptr2++ = *p_PtrTmp++ ;
    }
#endif

/* Syntax: void Swap( <Type> byref Var1,    <Type> byref Var2   [, <Type> byref varTemp] ) ;
 *         void Swap( <Type> *     PtrVar1, <Type> *     PtrVar2, <Type>* PtrVarTmp, size_t p_nSizeBytes ) ; */
#define Swap( _a, _b, ...) csname2(Swap_c_, Count_Args(__VA_ARGS__))( (_a), (_b), ##__VA_ARGS__)
#define Swap_c_0( _a, _b)                        swap_c(  &_a, &_b, sizeof(_a) )
#define Swap_c_1( _a, _b, _Tmp)                  ( (_Tmp)=(_a), (_a)=(_b), (_b)=(_Tmp))
#define Swap_c_2( _Ptr1, _Ptr2, _pTmp, _nBytes)  swap3_c( _Ptr1, _Ptr2, (_pTmp), (_nBytes) )
   
   

/* Syntax: void Swap( <Type> byref Var1,    <Type> byref Var2   [, <Type> byref varTemp] ) ;
 *         void Swap( <Type>*      PtrVar1, <Type>*      PtrVar2, size_t p_nSizeBytes, <Type>* PtrVarTmp ) ; */
   
// #define Swap( _a, _b, ...) csname2(Swap_c_, Count_Args(__VA_ARGS__))( _a, _b, ##__VA_ARGS__)
// #define Swap_c_0( _a, _b)                        swap_c(  (Byte*)&(_a), (Byte*)&(_b), sizeof(_a) )
// #define Swap_c_1( _a, _b, _tmp)                  swap3_c( (Byte*)&(_a), (Byte*)&(_b), sizeof(_a), (Byte*)&(_tmp) )
// #define Swap_c_2( _Ptr1, _Ptr2, _pTmp, _nBytes)  swap3_c( (Byte*)(_Ptr1), (Byte*)(_Ptr2), (Byte*)(_pTmp), (_nBytes) )

   
/* Syntax: void Swap( <Type> byref Var1,    <Type> byref Var2   [, <Type> byref varTemp] ) ;
 *         void Swap( <Type> *     PtrVar1, <Type> *     PtrVar2 , size_t p_nSizeBytes, <Type>* PtrVarTmp ) ; */

// ------------------------------------------------------------------
inline static void Randomize_0() ;
inline static void Randomize_1( long p_nValue ) ;
inline static int Random_0() ;
inline static int Random_1( unsigned p_nTop );

#define Randomize(...)  csname2(Randomize_, Count_Args(__VA_ARGS__))(__VA_ARGS__)

inline static void  Randomize_0() {
   srand( time(NULL) );
   rand(); rand();
}

inline static void Randomize_1( long p_nValue ) {
   if( p_nValue < 0 )
      srand( time(NULL) );
   else
      srand( p_nValue );
   rand(); rand();
}

#define Random(...)  csname2(Random_, Count_Args(__VA_ARGS__))(__VA_ARGS__)
inline static int Random_0() { return rand() ; }
inline static int Random_1( unsigned p_nTop ) { return rand() % p_nTop ; }

//--------------------------------------------------------------------

#define sizeofarray( array ) (sizeof(array)/sizeof(*(array)))

bool IsArraySorted( long* Array, int nSize, bool isAscending) {
   bool isSorted = true ;
   if( isAscending ) {
      for(int i=1; i < nSize; ++i )
         if( Array[i-1] > Array[i] )
            return false ;
   }
   else {
      for(int i=1; i < nSize; ++i )
         if( Array[i-1] < Array[i] )
            return false ;
   }
   return true;
}

// ------------------------------------------------------------------

#define show_array( _Type, _Mask, _Array, _Size ) \
   Procedure\
      if( !(_Size) ) return;\
      _Type *iter= (_Array),\
            *stop= (_Array) + (_Size) ;\
      printf("\n Array = { " _Mask, *iter );\
      for( ++iter ; iter < stop ; ++iter ) printf(", " _Mask, *iter);\
      printf(" }\n");\
   EndProcedure

//-------------------------[ Here define some Funtors Compare ]--------------------------------------

// the basic Funtors compare are defined after of quicksort algorithm.
#define FNCOMPARE( _Type )  _Type ## _Compare

#define  str_Compare strcmp
#define  string_Compare strcmp

inline int char_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(char*)p_ptr1 - *(char*)p_ptr2);
}
inline int uchar_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(uchar*)p_ptr1 - *(uchar*)p_ptr2);
}
inline int short_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(short*)p_ptr1 - *(short*)p_ptr2);
}
inline int ushort_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(ushort*)p_ptr1 - *(ushort*)p_ptr2);
}
inline int int_Compare(const void* p_ptr1, const void* p_ptr2) {
   return *(int*)p_ptr1 - *(int*)p_ptr2;
}
inline int uint_Compare(const void* p_ptr1, const void* p_ptr2) {
   return *(uint*)p_ptr1 - *(uint*)p_ptr2;
}
inline int long_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(long*)p_ptr1 - *(long*)p_ptr2);
}
inline int ulong_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(ulong*)p_ptr1 - *(ulong*)p_ptr2);
}
inline int llong_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(llong*)p_ptr1 - *(llong*)p_ptr2);
}
inline int ullong_Compare(const void* p_ptr1, const void* p_ptr2) {
   return (int)(*(ullong*)p_ptr1 - *(ullong*)p_ptr2);
}

inline int float_Compare(const void* p_ptr1, const void* p_ptr2) {
    float nA = *(float*)p_ptr1,
          nB = *(float*)p_ptr2,
          nDiff = nA - nB ;
    #ifdef _WIN32
    nA = (float)fabs(nA);
    nB = (float)fabs(nB);
    return  (float)fabs( nDiff ) <= Max2(nA,nB)*FLT_EPSILON ? 0 : (nDiff<0.0f ? -1 : 1) ;
    #else
    nA = fabsf(nA);
    nB = fabsf(nB);
    return  fabsf( nDiff ) <= Max2(nA,nB)*FLT_EPSILON ? 0 : (nDiff<0.0f ? -1 : 1) ;
    #endif
}
inline int double_Compare(const void* p_ptr1, const void* p_ptr2) {
    double nA = *(double*)p_ptr1,
           nB = *(double*)p_ptr2,
           nDiff = nA - nB ;
    nA = fabs(nA);
    nB = fabs(nB);
    return  fabs( nDiff ) <= Max2(nA,nB)*DBL_EPSILON ? 0 : (nDiff<0.0 ? -1 : 1) ;
}
// #endif

// ------------------------------------------------------------------

#define QSort( _Array, _nLength, _Type, ...) csname2(QSort_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), FNCOMPARE(_Type), ##__VA_ARGS__)
#define QSort_0( _Array, _nLength, _fnCompare )               QSort_1( _Array, _nLength, _fnCompare, ASC )
#define QSort_1( _Array, _nLength, _fnCompare, _bIsAscending) \
   Procedure\
      if( 1 < _nLength ) { \
         if( _bIsAscending ) \
            qsort( (void*)_Array, _nLength, sizeof(*_Array), _fnCompare ); \
         else \
            qsort( (void*)_Array, _nLength, sizeof(*_Array), _fnCompare ); \
      } \
   EndProcedure


//--------------------------------------------------------------------

#ifdef __cplusplus
    extern "C" {
#endif

// Get time in micro seconds.
inline llong time_usec() {
   llong microsec = -1;
   #ifdef _WIN32
      FILETIME  tInfo;
      GetSystemTimeAsFileTime( &tInfo );
      microsec = tInfo.dwHighDateTime;
      microsec <<=32;
      microsec |= tInfo.dwLowDateTime;
      microsec /=10;
      microsec -= 11644473600000000ULL;
   #else
      timeval  tInfo;
      if( !gettimeofday( &tInfo, NULL ) )
         microsec = (llong) tInfo.tv_sec * 1000000ll +
                    (llong) tInfo.tv_usec ;
   #endif
   return microsec ;
}
inline llong time_nsec() {
   llong nanosec = -1;
   #ifdef _WIN32
      FILETIME  tInfo;
      GetSystemTimeAsFileTime( &tInfo );
      nanosec = tInfo.dwHighDateTime;
      nanosec <<=32;
      nanosec |= tInfo.dwLowDateTime;
      nanosec -= 1164447360000000ULL;
   #else
      timeval  tInfo;
      if( !gettimeofday( &tInfo, NULL ) )
         nanosec = ( (llong) tInfo.tv_sec * 1000000ll +
                      (llong) tInfo.tv_usec ) * 10ll ;
   #endif
   return nanosec ;
}

#ifdef __cplusplus
    }
#endif

#define Time_Elapsed_usec( out_strTime, _a, ...) csname2(Time_Elapsed_usec_, Count_Args(__VA_ARGS__))(out_strTime, _a, ##__VA_ARGS__)
char*    Time_Elapsed_usec_0(char* strTime, llong p_nElapsedTime ) {
   llong nsign = p_nElapsedTime < 0 ? -1LL : 1LL,
         usec,
         yy, dd,
         hh, mm, ss ;
   p_nElapsedTime *= nsign ;
   usec = p_nElapsedTime % 1000000ll,
   p_nElapsedTime  = (p_nElapsedTime - usec) / 1000000ll ; // Here time is in seconds
   yy = p_nElapsedTime / 31536000 ;   // Secons per year
   p_nElapsedTime -= yy * 31536000 ;
   dd = p_nElapsedTime / 86400 ;      // Secons per day
   p_nElapsedTime -= dd * 86400 ;
   hh = p_nElapsedTime / 3600 ;       // Secons per hour
   p_nElapsedTime -= hh * 3600 ;
   mm = p_nElapsedTime / 60 ;         // Seconds per minute
   p_nElapsedTime -= mm * 60 ;
   ss  = p_nElapsedTime  ;            // Seconds
   int bytes = 0;
   char* szPos = strTime ;
   if( yy )   { szPos += bytes; bytes = sprintf(szPos, " %d years", yy) ; }
   if( dd )   { szPos += bytes; bytes = sprintf(szPos, " %d days", dd) ; }
   if( hh )   { szPos += bytes; bytes = sprintf(szPos, " %d h", hh) ; }
   if( mm )   { szPos += bytes; bytes = sprintf(szPos, " %d m", mm) ; }
   if( ss )   { szPos += bytes; bytes = sprintf(szPos, " %d s", ss) ; }
   if( usec ) { szPos += bytes; bytes = sprintf(szPos, " %0.3f ms", usec/1000.0 ); }
}
inline void  Time_Elapsed_usec_1( char* strTime, llong p_nBeginTime, llong p_nEndTime ){
   Time_Elapsed_usec_0( strTime,
                          p_nBeginTime > p_nEndTime
                        ? p_nBeginTime - p_nEndTime
                        : p_nEndTime -   p_nBeginTime ) ;
}


#ifdef __cplusplus
    }
#endif

//--------------------------------------------------------------------
//--------------------------------------------------------------------
