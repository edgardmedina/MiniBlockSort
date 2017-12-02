/********************************************************************************************************
 * @Objetive : Provide generic functions to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             functions to manipulate memroy load the standard header for
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 ********************************************************************************************************
 *  Constants:
 *  ---------
 *
 *  const int  ASC=true;
 *  const int  DESC=false;
 *
 *  Numeric Function
 *  ----------------
 *  int isOdd(  long nNumber )      // Retorna un boolean, verifica si un numero es impar.
 *  int esImpar(  long nNumber )    // Idem  isOdd().
 *  int isEven( long nNumber )      // Retorna un boolean, verifica si un numero es par.
 *  int esPar(  long nNumber )      // Idem isEven()
 *
 *
 *
 * Miscellaneous Template Functions :
 * --------------------------------
 *  <Type> Min2( <Type> a, <Type> b  )  : Get minimun value of 'a' or 'b' number values,
 *  <Type> Max2( <Type> a, <Type> b  )  : Get maximun value of 'a' or 'b' number values,
 *
 * Get Middle Point in a segment
 * -----------------------------
 *  <TInteger> Middle( <TInteger> nNumber ) 
 *  <TInteger> Middle( <TInteger> nLow,  <TInteger> nHigh ) 
 *       void* Middle(      void* ptrLow,     void* ptrHigh, size_t nSizeBytes ) 
 *
 *  void Swap( <Type> a, <Type> b [, <Type> tmp] )  : Swapped two values, optionally let to use a
 *                                                    a extern temporal variable for best performance
 *                                                    in loops.
 *  void Swapif( <Bool_Expresssion>,  <Type> a, <Type> b [, <Type> tmp] )
 *
 * Random Functions (Basic)
 * ------------------------
 *  void Randomize( [size_t nSeed_Value [, NULL] ] )  : Initialize the seed for Random() functions.
 *                                              nSeed_Value:  {empty}  Generate a random value from system clock.
 *                                                            n < 0   Generate a random value from system clock.
 *                                                            0 <= n  Generate a random value from 'n' base number.
 *                                              NULL       : If specified this parameter, the seed is more fuzzied.
 *
 *  long  Rand()                         : Generate random number between 0 and 65536 (2^16)
 *  long  Rand( long nTopNum )           : Generate random number between 0 and {nTopNum}-1, where {nTopNum} <= 65536 (2^16)
 *  long  Rand( long nMin, long nMax] )  : Generate random number between {nMin} and {nMax}, where (-2^16-1) -65535 <= {nMin} <= {nMax} <= 65536 (2^16)
 *
 * Random Functions (ADVANCED) (see "ebm_random.h")
 * ------------------------------------------------
 *  <TNumber> Random( <TNumber> );                                                              // Random between 0 .. RAND_MAX_{TYPE}
 *  <TNumber> Random( <TNumber>, <TNumber> p_nTop);                                             // Random between 0 .. (p_nTop-1),  p_nTop <= RAND_MAX_{TYPE}
 *  <TNumber> Random( <TNumber>, <TNumber> p_nMin, <TNumber> p_nMax);                           // Random between 0 .. p_nMax,      p_nMax <= RAND_MAX_{TYPE}
 *  <TNumber> Random( <TNumber>, <TNumber> p_nMin, <TNumber> p_nMax, <TNumber> p_nMultiple_Of ) // Random between 0 .. p_nMax,  p_nMax <= RAND_MAX_{TYPE}
 *
 *
 * Array Functions:
 * ----------------
 *  * Check out if array ir sorted in ascendig mode or descending mode.
 *     bool  IsArraySorted( long* Array, int nSlotsArray, bool isAscending) :
 *     void  Check_Sorted(  long* Array, int nSlotsArray, bool isAscending) :
 *
 *  * Template procedure to show an array on screen
 *     void  show_array( <Type>, const char* strMask, <type>* Array, size_t nSlotsArray )
 *
 *  * Function get quantity of slots in Static Array. NOTE: don't work in Dynamic Arrays.
 *     int sizeofarray( Static_Array )
 *     int arraysize( Static_Array )
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
#ifndef __ebmq_base_h__
#define __ebmq_base_h__

//------------------------------------------------------------------------
// for use C template functions for basic types.. The USER if want use this, then he
// must use '#define C_TEMPLATE' before '#include "ebm_base.h"'

// --------------------[ Defining Global resources ]--------------------
//------------------------------------------------------------------------

#ifdef __cplusplus
    extern "C" {
#endif

enum  {
#ifndef IGNORE
      IGNORE=0,
#endif
    NOIGNORE=1
} ;

 

#define esPar   isEven
#define esImpar isOdd
#define isEven( _nNum )    !(1L & (_nNum))
#define isOdd(  _nNum )     (1L & (_nNum))
// inline int isEven(long p_nNum) { return !(1L & p_nNum) ; }
// inline int isOdd( long p_nNum) { return 1L & p_nNum ; }

// The macros 'min()' and 'max()' from stdlib.h are not safed, 
// By this reason I prefer my construction 'Min2()' and 'Max2()'.
#define Min2(a,b)     Function typeof(a) _a = (a), _b = (b);  _a < _b ? _a : _b; EndFunction
#define Max2(a,b)     Function typeof(a) _a = (a), _b = (b);  _a < _b ? _b : _a; EndFunction

// Middle Function, it use of right shift (>>1) of 1. Note: N>>1 its better that N/2.
#define Middle( _nValue, ...) csname2(Middle_, Count_Args(__VA_ARGS__))( _nValue,  ##__VA_ARGS__ )
#define Middle_0( _Number )            ( (_Number) >> 1 ) 
#define Middle_1( _nLow, _nHigh )    ( (_nLow) + (( (_nHigh) - (_nLow)) >> 1) )
#define Middle_2( _ptrLow, _ptrHigh, _nSizeBytes ) \
        ((_ptrLow) + (_nSizeBytes) * (((_ptrHigh) - (_ptrLow)) / (_nSizeBytes) >> 1))

#define Trunc(nVal) ((typeof(nVal))(llong)(nVal))

// Modulus / Remainer works with any numeric datatype.
#define  mod( nVal1, nVal2 ) \
    Function \
        typeof(nVal1) _n1 = Trunc( (nVal1) ), \
                      _n2 = Trunc( (nVal2) ); \
        ( _n1 - _n2 * Trunc( _n1 / _n2 ) ); \
    EndFunction

    
// ------------------------------------------------------------------

#define PtrVal( _Type, _Param, ...) csname2(PtrVal_, Count_Args(__VA_ARGS__))( _Type, _Param, ##__VA_ARGS__)
#define PtrVal_0( _Type, _pDatum )                  (*((_Type*)(_pDatum)))
#define PtrVal_1( _Type,         _pObject, _Field)  (PtrVal_0(_Type,  _pObject)._Field)
#define PtrVal_2( _Type, ptrRef, _pObject, _Field)  (PtrVal_0(_Type*, _pObject)->_Field)

// For function comparing Compare_0(a,b)=(a-b) is deprecated, because can cause overflow and isn't safe,
// better is use the form Compare_0(a,b)=((a > b) - (a < b)), it is safe function. For more options see: 
//   https://stackoverflow.com/questions/10996418/efficient-integer-compare-function
#define Compare( _Param1, _Param2, ...) csname2(Compare_, Count_Args(__VA_ARGS__))( _Param1, _Param2, ##__VA_ARGS__)
#define Compare_0( _Num1, _Num2)                (((_Num1) > (_Num2)) - ((_Num1) < (_Num2)))
#define Compare_1( _Type, _PtrNum1, _PtrNum2)              Compare_0( *((_Type*)(_PtrNum1)) , \
                                                                      *((_Type*)(_PtrNum2)) )
#define Compare_2( _Type,         _pObj1, _pObj2, _field ) Compare_0( PtrVal_0( _Type, (_pObj1))._field ,\
                                                                      PtrVal_0( _Type, (_pObj2))._field )
#define Compare_3( _Type, _PtrRef,_pObj1, _pObj2, _field ) Compare_0( PtrVal_0( _Type*, (_pObj1))->_field ,\
                                                                      PtrVal_0( _Type*, (_pObj2))->_field )



// ------------------------------------------------------------------

#ifdef EBM_TEMPLATE
    #define  swap_c( _Ptr1, _Ptr2, _nBytes ) \
       Procedure\
          Byte  _cTmp,\
               *_ptr1 = _Ptr1,\
               *_ptr2 = _Ptr2;\
          register ushort _nSBytes = _nBytes ;\
          do {\
                _cTmp = *_ptr1 ,\
             *_ptr1++ = *_ptr2 ,\
             *_ptr2++ = _cTmp ;\
          } while( --_nSBytes > 0 );\
       EndProcedure
#else
    inline void  swap_c(  register Byte* p_Ptr1
                        , register Byte* p_Ptr2
                        , register ushort p_nBytes
    ) {
       register Byte   cTmp;
       while( p_nBytes-- )
               cTmp = *p_Ptr1,
          *p_Ptr1++ = *p_Ptr2,
          *p_Ptr2++ = cTmp ;
    }
#endif


/* Syntax: void Swap( <Type> byref Var1,    <Type> byref Var2   [, <Type> byref varTemp] ) ;
 *         void Swap( <Type> *     PtrVar1, <Type> *     PtrVar2, <Type>* PtrVarTmp, size_t p_nSizeBytes ) ; */
#define Swap( _a, _b, ...) csname2(Swap_c_, Count_Args(__VA_ARGS__))( (_a), (_b), ##__VA_ARGS__)
#define Swap_c_0( _a, _b)                         swap_c(  (Byte*)&(_a), (Byte*)&(_b), sizeof(_a) )
#define Swap_c_1( _a, _b, _Tmp)                   ( (_Tmp)=(_a), (_a)=(_b), (_b)=(_Tmp))
#define Swap_c_2( _Ptr1, _Ptr2, _pTmp, _nBytes)   swap3_c( _Ptr1, _Ptr2, (_pTmp), (_nBytes) )

#ifdef EBM_TEMPLATE
    #define swap3_c( _Ptr1,  _Ptr2, _PtrTmp, _nBytes) \
       Procedure\
          register Byte *_pTmp = (Byte *)_PtrTmp,\
                        *_ptr1 = (Byte *)_Ptr1,\
                        *_ptr2 = (Byte *)_Ptr2;\
          register ushort _nSBytes = (ushort)(_nBytes) ;\
          while( _nSBytes--) \
             *_pTmp   = *_ptr1 ,\
             *_ptr1++ = *_ptr2 ,\
             *_ptr2++ = *_pTmp++ ;\
       EndProcedure
#else
    inline register void  swap3_c(  register Byte* p_Ptr1
                         , register Byte* p_Ptr2
                         , register Byte* p_PtrTmp
                         , register ushort p_nBytes
    ) {
       while( p_nBytes-- )
          *p_PtrTmp = *p_Ptr1,
          *p_Ptr1++ = *p_Ptr2,
          *p_Ptr2++ = *p_PtrTmp++ ;
    }
#endif

#define Swapif(   _Cond, _a, _b, ...) if( _Cond ) Swap(_a, _b, ##__VA_ARGS__)


/* Syntax: void Swap( <Type> byref Var1,    <Type> byref Var2   [, <Type> byref varTemp] ) ;
 *         void Swap( <Type>*      PtrVar1, <Type>*      PtrVar2, size_t p_nSizeBytes, <Type>* PtrVarTmp ) ; */

// #define Swap( _a, _b, ...) csname2(Swap_c_, Count_Args(__VA_ARGS__))( _a, _b, ##__VA_ARGS__)
// #define Swap_c_0( _a, _b)                        swap_c(  (Byte*)&(_a), (Byte*)&(_b), sizeof(_a) )
// #define Swap_c_1( _a, _b, _tmp)                  swap3_c( (Byte*)&(_a), (Byte*)&(_b), sizeof(_a), (Byte*)&(_tmp) )
// #define Swap_c_2( _Ptr1, _Ptr2, _pTmp, _nBytes)  swap3_c( (Byte*)(_Ptr1), (Byte*)(_Ptr2), (Byte*)(_pTmp), (_nBytes) )


/* Syntax: void Swap( <Type> byref Var1,    <Type> byref Var2   [, <Type> byref varTemp] ) ;
 *         void Swap( <Type> *     PtrVar1, <Type> *     PtrVar2 , size_t p_nSizeBytes, <Type>* PtrVarTmp ) ; */

// ------------------------------------------------------------------

 #define log_info(_Frmt, ...) fprintf(stderr, \
                        "[INFO] [%s: Lin=%d: \'%s()\'] " _Frmt "\n", \
                        __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

// ------------------------------------------------------------------
#define MASK_1BITS  0x00000001
#define MASK_2BITS  0x00000003
#define MASK_3BITS  0x00000007
#define MASK_4BITS  0x0000000F
#define MASK_5BITS  0x0000001F
#define MASK_6BITS  0x0000003F
#define MASK_7BITS  0x0000007F
#define MASK_8BITS  0x000000FF

#define MASK_9BITS  0x000001FF
#define MASK_10BITS 0x000003FF
#define MASK_11BITS 0x000007FF
#define MASK_12BITS 0x00000FFF
#define MASK_13BITS 0x00001FFF
#define MASK_14BITS 0x00003FFF
#define MASK_15BITS 0x00007FFF
#define MASK_16BITS 0x0000FFFF

#define MASK_17BITS 0x0001FFFF
#define MASK_18BITS 0x0003FFFF
#define MASK_19BITS 0x0007FFFF
#define MASK_20BITS 0x000FFFFF
#define MASK_21BITS 0x001FFFFF
#define MASK_22BITS 0x003FFFFF
#define MASK_23BITS 0x007FFFFF
#define MASK_24BITS 0x00FFFFFF

#define MASK_25BITS 0x01FFFFFF
#define MASK_26BITS 0x03FFFFFF
#define MASK_27BITS 0x07FFFFFF
#define MASK_28BITS 0x0FFFFFFF
#define MASK_29BITS 0x1FFFFFFF
#define MASK_30BITS 0x3FFFFFFF
#define MASK_31BITS 0x7FFFFFFF
#define MASK_32BITS 0xFFFFFFFF

#if not defined(INT8_MIN) && not defined(INT8_MAX)
    #define INT8_MIN (-128)
    #define INT16_MIN (-32768)
    #define INT32_MIN (-2147483647 - 1)
    #define INT64_MIN  (-9223372036854775807LL - 1)

    #define INT8_MAX  127
    #define INT16_MAX 32767
    #define INT32_MAX 2147483647
    #define INT64_MAX 9223372036854775807LL

    #define UINT8_MAX 0xffu /* 255U */
    #define UINT16_MAX 0xffffu /* 65535U */
    #define UINT32_MAX 0xffffffffU  /* 4294967295U */
    #define UINT64_MAX 0xffffffffffffffffULL /* 18446744073709551615ULL */
#endif

#define BYTE_MAX    UINT8_MAX
#define WORD_MAX    UINT16_MAX
#define DWORD_MAX   UINT32_MAX
#define QWORD_MAX   UINT64_MAX

#define ALL  INT32_MAX
// ------------------------------------------------------------------

static ulong __mixTime_(ulong a, ulong long b, ulong c) {
    a-=b;  a-=c;  a^=(c >> 13);
    b-=c;  b-=a;  b^=(a << 8);
    c-=a;  c-=b;  c^=(b >> 13);
    a-=b;  a-=c;  a^=(c >> 12);
    b-=c;  b-=a;  b^=(a << 16);
    c-=a;  c-=b;  c^=(b >> 5);
    a-=b;  a-=c;  a^=(c >> 3);
    b-=c;  b-=a;  b^=(a << 10);
    c-=a;  c-=b;  c^=(b >> 15);
    return c;
}

static uint __time_seed() {
    time_t tTmp, 
           now = time(&tTmp);
    uchar* p = (uchar*)&now;
    unsigned nSeed = 0;
    for (size_t i = 0; i < sizeof( now); ++i) nSeed = nSeed * (UCHAR_MAX + 2U) + p[i];
    return nSeed;
}
#define Randomize(...) csname2(Randomize_,Count_Args(__VA_ARGS__))(__VA_ARGS__)
#define Randomize_0() Procedure time_t  t; srand( __time_seed(&t) ); rand(); EndProcedure
#define Randomize_1(_nSeed)        Procedure time_t  t; srand( (size_t)(_nSeed) ); rand(); EndProcedure
#define Randomize_2(_nSeed, _time) Procedure time_t  t; srand( __mixTime_( clock(), time(&t), GetPID()) ); rand(); EndProcedure

#define Rand(...)  csname2(Rand_, Count_Args(__VA_ARGS__))(__VA_ARGS__)
inline static long Rand_0() { return rand() ; }
inline static long Rand_1( long p_nTop ) { return rand() % p_nTop ; }
inline static long Rand_2( long p_nMin, long p_nMax ) { return p_nMin + Rand(p_nMax-p_nMin+1) ; }
/*-------------------------------------------------------------------------*/

//--------------------------------------------------------------------

#ifdef __cplusplus
    }
#endif

#endif // end #define __ebmq_base_h__
