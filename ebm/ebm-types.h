/********************************************************************************************************
 * @Objetive : Provide generic MACROS for macro programming, generic programming
 *             in GNU C/C++.
 *             The minimun requirement is GNU C90 or GNU C++94.
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************/

#ifndef __ebm_types_h__
#define __ebm_types_h__
typedef unsigned char          Byte, *lpByte;
typedef unsigned char          byte, uchar;
typedef unsigned short         ushort;
typedef unsigned int           uint;
typedef unsigned long          ulong;
typedef long long int          llong;
typedef unsigned long long int ullong;
typedef long double            ldouble;
typedef char                   *lpstring ;
typedef wchar_t                *lpwstring ;

#define FMT( _Type )  CSname2(__FMT_TYPE_,  _Type )

/*****[ String Mask ]*****/
#define __FMT_TYPE_str     "%s"
#define __FMT_TYPE_string  "%s"
#define __FMT_TYPE_lpstring  "\"%s\""
// #define __FMT_TYPE_lpwstring "\"%ls\""

#define __FMT_TYPE_char     "%c"
#define __FMT_TYPE_uchar    "%c"
#define __FMT_TYPE_WUChar   "%lc"

/*****[ Pointer Mask ]*****/
#define __FMT_TYPE_upointer    "%p"
#define __FMT_TYPE_nulpointer  "%n"

#define __FMT_TYPE_lpwstring "\"%s\""
#define __FMT_TYPE_bool     "%i"
#define __FMT_TYPE_Bool     "%i"
#define __FMT_TYPE__Bool    "%i"

/*****[ Integer Mask ]*****/
// [ Decimal Integer Base ]
#define __FMT_TYPE_byte    "%hhd"
#define __FMT_TYPE_short   "%hd"
#define __FMT_TYPE_int     "%d"
#define __FMT_TYPE_long    "%d"
#define __FMT_TYPE_llong   "%lld"

// [ Unsigned Integer Base ]
#define __FMT_TYPE_ubyte    "%hhu"
#define __FMT_TYPE_ushort   "%hu"
#define __FMT_TYPE_uint     "%u"
#define __FMT_TYPE_ulong    "%u"
#define __FMT_TYPE_ullong   "%llu"

// [ Float Mask ]
#define __FMT_TYPE_float   "%.14g"
#define __FMT_TYPE_double  "%.14g"
#define __FMT_TYPE_ldouble "%.14Lg"


// -------[ boolean operator equivalent to "iso646.h" (if is avaliable in compiler)]
#ifndef and
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
#endif
// Bit operators
#define Not     compl
#define And     bitand
#define Or      bitor
#define Xor     xor

#define And_eq  and_eq
#define Or_eq   or_eq
#define Xor_eq  xor_eq


#define sizeofarray( _Static_Array ) (sizeof(_Static_Array)/sizeof(*(_Static_Array)))
#define arraysize( _Static_Array )  sizeofarray(_Static_Array)


#ifdef __cplusplus
    extern "C" {
#endif

// --------------------[ Private macros for C Templates ]--------------------

#if defined(__INTEL_COMPILER) or defined(__BORLANDC__) or defined(__GNUC__) \
    or defined(__MINGW32__) or defined(__MINGW64__)
    #define __EXTENSION__   __extension__
#endif

#ifndef __EXTENSION__
    #define __EXTENSION__
#endif


#define Function    __EXTENSION__({
#define EndFunction })

#define Procedure     do {
#define EndProcedure  } while(0)

#define _STR( _Token ) #_Token
#define STR( _Token ) _STR( _Token )

//---------------------------------------------
#define csname2_( _a, _b )                 _a ## _b
#define csname3_( _a, _b, _c )             _a ## _b ## _c

#define csname2(  _a, _b )         csname2_( _a, _b )
#define csname3(  _a, _b, _c )     csname3_( _a, _b, _c )

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

#ifdef __cplusplus
    }
#endif

// --------------------[ Public macros and methods ]--------------------
/***********************************************************************************************
 * Define a loop structure 'Foreach()'
 * Syntax:   Foreach( <datatype>, <var_Iterator>, <value1> [, <value2>, ... , <valueN>] ) {
 *              // ...   So something
 *           }
 ***********************************************************************************************/
#define Foreach( _Type, _Iter, _TypeFor, _Item, ... ) Foreach_##_TypeFor( _Type, _Iter, _Item, ##__VA_ARGS__ )

#define Foreach_inlist( _Type, _Iter, _Item, ... ) \
      for( _Type _arrList[] = {_Item, ##__VA_ARGS__}, \
                *_InIter = _arrList, \
                *_Stop   = _InIter + sizeofarray(_arrList), \
                 _Iter   = *_InIter \
          ; _InIter < _Stop \
          ; _Iter = *(++_InIter) ) {

#define End( _TypeFor, ... ) End_ ## _TypeFor( __VA_ARGS__ )

#define End_inlist() }

#define Do


// ---------------------------------------------------------------------------

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


#endif  //  #ifndef __ebm_types_h__
