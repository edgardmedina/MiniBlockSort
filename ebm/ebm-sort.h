/********************************************************************************************************
 * @Objetive : Provide generic functions to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             functions to manipulate memroy load the standard header for
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 ********************************************************************************************************
 *   Constants           Value
 *  ----------------    --------------
 *   ASC                    true
 *   DESC                   false
 *   gszMethodAsc        "Ascending"
 *   gszMethodDes        "Descending"
 *
 *   Function Types      Definition
 *  ----------------    ---------------------------------------
 *   fnCompare_t         int (*)(const void*, const void*)
 
 *   <Type> PtrVal( <Type>,       void* ptrValue )                  // Get a value from a generic pointer to Datum.
 *   <Type> PtrVal( <TObject>,    void* ptrObject1, <Type> Field )  // Get a value from a generic pointer to Object Strcutrure's field. Also, it is equivalent to
 *                                                                  //   PtrVal(<TObject>, PtrObject1, Field ) == PtrVal(<TObject>, PtrObject1).Field
 *   <Type> PtrVal( <TObject>, *, void* ptrObject1, <Type> Field )  // Get a value from a generic pointer to pointer of Object Strcutrure's field. Also, it is equivalent to
 *                                                                  //   PtrVal(<TObject>, *, PtrObject1, Field ) == PtrVal(<TObject>*, PtrObject1)->Field
 *  
 *   int Compare(               <TNumber> Number1,   <TNumber> Number2    )                  // Compare two numbers values
 *   int Compare( <TNumber>,    void*     PtrNumber1, void*    PtrNumber2 )                  // Compare two numbers values from generic pointers to Data, both pointer object are the same type.
 *   int Compare( <TObject>,    void*     PtrObject1, void*    PtrObject2, <TNumber> Field ) // Compare two numbers values from generic pointers to Object Structure, both pointer object are the same type.
 *   int Compare( <TObject>, *, void*     PtrObject1, void*    PtrObject2, <TNumber> Field ) // Compare two numbers values from generic pointers to pointer to Object Structure, both pointer object are the same type.
 * 
 *   Predefined Compare Functions of type 'fnCompare_t'
 *  ----------------------------------------------------------------------------
 *   FNCOMPARE( <Type> )   ==>  int  <Type>_Compare(const void* Pointer1, const void* Pointer2)
 *
 *          Type ::= str    | string
 *                 | char   | uchar
 *                 | short  | ushort
 *                 | int    | uint
 *                 | long   | ulong
 *                 | llong  | ullong
 *                 | float  | double 
 *  
 *   Quick Sort Macro Template
 *  ----------------------------------------------------------------- 
 *   QSort(    <Type>* Array, size_t  nSlots [ [, <Type>],  bool IsAscending = true] )
 *   QSort_fn( <Type>* Array, size_t  nSlots, fnCompare_t Funtor_Compare [, bool IsAscending = true] )
 *  
 *   Funtions for Arrays
 *  ----------------------------------------------------------------- 
 *   bool IsArraySorted( <Type>, <Type> pArray, size_t nSlotsInArray                        [. bool isAscending = true] )
 *   bool IsArraySorted_fn(      <Type> pArray, size_t nSlotsInArray, fnCompare_t fnCompare [, bool isAscending = true] )
 *  
 *   bool Check_Sorted(  <Type>, <Type> pArray, size_t nSlotsInArray,                       [, bool isAscending = true] )
 *   bool Check_Sorted_fn(       <Type> pArray, size_t nSlotsInArray, fnCompare_t fnCompare [, bool isAscending = true] )
 *  
 *   void show_array( <Type>, strMaskType, pArray, nSlots ) 
 *  
 ********************************************************************************************************/

#ifndef __ebm_sort_h__
#define __ebm_sort_h__

#include "ebm-random.h"
//-------------------------[ Here define some Funtors Compare ]--------------------------------------
const int  ASC=true;
const int  DESC=false;
char gszMethodAsc[] = "Ascending";
char gszMethodDes[] = "Descending";


/*
 *  If a > b --> fnCompare_t( a, b ) > 0
 *  If a = b --> fnCompare_t( a, b ) = 0
 *  If a < b --> fnCompare_t( a, b ) < 0
 */
typedef int (*fnCompare_t)(const void*, const void*);

#ifdef __cplusplus
    extern "C" {
#endif


#define    str_Compare  strcmp
#define string_Compare  strcmp

// the basic Funtors compare are defined after of quicksort algorithm.
#define FNCOMPARE( _Type )  _Type ## _Compare

inline int   char_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(   char, p_ptr1, p_ptr2); }
inline int  uchar_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(  uchar, p_ptr1, p_ptr2); }
inline int  short_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(  short, p_ptr1, p_ptr2); }
inline int ushort_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare( ushort, p_ptr1, p_ptr2); }
inline int    int_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(    int, p_ptr1, p_ptr2); }
inline int   uint_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(   uint, p_ptr1, p_ptr2); }
inline int   long_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(   long, p_ptr1, p_ptr2); }
inline int  ulong_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(  ulong, p_ptr1, p_ptr2); }
inline int  llong_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare(  llong, p_ptr1, p_ptr2); }
inline int ullong_Compare(const void* p_ptr1, const void* p_ptr2)  { return Compare( ullong, p_ptr1, p_ptr2); }

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

#define QSort(   _Array, _nSlots, ...) csname2(QSort_, Count_Args(__VA_ARGS__))( (_Array), (_nSlots), ##__VA_ARGS__)
#define QSort_0( _Array, _nSlots)                        QSort_fn_1( _Array, _nSlots, FNCOMPARE(long), ASC )
#define QSort_1( _Array, _nSlots, _bIsAscending)         QSort_fn_1( _Array, _nSlots, FNCOMPARE(long), _bIsAscending)
#define QSort_2( _Array, _nSlots, _Type, _bIsAscending)  QSort_fn_1( _Array, _nSlots, FNCOMPARE(_Type), _bIsAscending )


#define QSort_fn(   _Array, _nSlots, _fnCompare, ...) csname2(QSort_fn_, Count_Args(__VA_ARGS__))( (_Array), (_nSlots), _fnCompare, ##__VA_ARGS__)
#define QSort_fn_0( _Array, _nSlots, _fnCompare)              QSort_fn_1( _Array, _nSlots, _fnCompare, ASC )
#define QSort_fn_1( _Array, _nSlots, _fnCompare, _bIsAscending) \
   Procedure\
      if( 1 < _nSlots ) { \
         if( _bIsAscending ) {\
            qsort( (void*)_Array, _nSlots, sizeof(*_Array), _fnCompare ); \
         }\
         else {\
            printf("   --> QSort is Not Posible sorted in Descending Mode.\n");\
            qsort( _Array, _nSlots, sizeof(*_Array), _fnCompare ); \
        }\
      } \
   EndProcedure

//--------------------------------------------------------------------
/*
void __InsertSort_Asc( void* pLow, void* pHigh, const int p_nSizeByte, fnCompare_t fnCompare );
void __InsertSort_Desc(void* pLow, void* pHigh, const int p_nSizeByte, fnCompare_t fnCompare );

#define ISort(   _Array, _nSlots, ...)         csname2(ISort_, Count_Args(__VA_ARGS__))( (_Array), (_nSlots), ##__VA_ARGS__)
#define ISort_0( _Array, _nSlots)                        _InsertSort( _Array, _nSlots, sizeof(*_Array), FNCOMPARE(long),  ASC )
#define ISort_1( _Array, _nSlots, _bIsAscending)         _InsertSort( _Array, _nSlots, sizeof(*_Array), FNCOMPARE(long),  _bIsAscending )
#define ISort_2( _Array, _nSlots, _Type, _bIsAscending)  _InsertSort( _Array, _nSlots, sizeof(*_Array), FNCOMPARE(_Type), _bIsAscending )

#define ISort_fn(   _Array, _nSlots, _fnCompare, ...)         csname2(ISort_fn_, Count_Args(__VA_ARGS__))( (_Array), (_nSlots), _fnCompare, ##__VA_ARGS__)
#define ISort_fn_0( _Array, _nSlots, _fnCompare )                       _InsertSort( _Array, _nSlots, sizeof(*_Array), _fnCompare,  ASC )
#define ISort_fn_1( _Array, _nSlots, _fnCompare, _bIsAscending)         _InsertSort( _Array, _nSlots, sizeof(*_Array), _fnCompare,  _bIsAscending )

inline void _InsertSort(void* pArray, size_t p_nSlots, size_t p_nSizeByte, fnCompare_t fnCompare, bool p_bAscending ) {
   if (p_nSlots < 2) return;
   
   if( p_bAscending )
      __InsertSort_Asc( pArray, pArray+(p_nSlots-1)*p_nSizeByte, p_nSizeByte, fnCompare);
   else
      __InsertSort_Desc(pArray, pArray+(p_nSlots-1)*p_nSizeByte, p_nSizeByte, fnCompare);
}

// ...............................................

void __InsertSort_Asc( void* p_pLow, void* p_pHigh, const int p_nSizeByte, fnCompare_t fnCompare ) {
   char *pStop = (char*)p_pHigh + 1,
        *pLow  = (char*)p_pLow;
   char pTemp[p_nSizeByte];
   for (char* pIter1 = pLow + p_nSizeByte; pIter1 < pStop; pIter1+= p_nSizeByte ) 
      for (char *pIter2 = pIter1, 
                *pPrior=pIter2-p_nSizeByte
          ; pLow < pIter2  &&  fnCompare(pPrior, pIter2) > 0
          ; pIter2-=p_nSizeByte, pPrior=pIter2-p_nSizeByte ) 
         Swap( pPrior, pIter2, pTemp, p_nSizeByte );
}
/**/


//--------------------------------------------------------------------

inline size_t  MinOfMax_Search( long p_nValue, long p_pArray[], size_t p_nLow, size_t p_nHight )  {
    if( p_nValue < p_pArray[p_nLow]   ) return p_nLow;
    if( p_pArray[p_nHight] <= p_nValue) return p_nHight + 1;
    size_t nMidle;
    while( p_nLow < p_nHight ) {
        nMidle = Middle( p_nLow,  p_nHight) ;
        if( p_nValue < p_pArray[nMidle] ) 
            p_nHight = nMidle ;
        else {
            p_nLow = nMidle+1 ;
            if( p_nValue < p_pArray[p_nLow] ) return p_nLow ;
        }
    }
    return p_nLow + ( p_nValue < p_pArray[p_nLow] ? 0 : 1);
}

static inline size_t  Search_MinOfMax2( long p_nValue, long p_pArray[], size_t p_nLow, size_t p_nHight )  {
    if( p_nValue < p_pArray[p_nLow]   ) return p_nLow;
    if(  p_pArray[p_nHight] <= p_nValue) return p_nHight + 1;
    size_t nMidle;
    while( p_nLow+1 < p_nHight ) {
        nMidle = Middle( p_nLow, p_nHight) ;
        if( p_nValue < p_pArray[nMidle]  ) {
            if( p_nValue < p_pArray[nMidle-1] ) 
                p_nHight = nMidle-1 ;
            else
                return nMidle ;
        } else 
            p_nLow = nMidle ;
    }
    return p_nLow + (p_nValue < p_pArray[p_nLow] ? 0: 1);
}
   
//--------------------------------------------------------------------

#define  IsArraySorted_fn( _pArray, _nSlots, _fnCompare, ...)    csname2(IsArraySorted_, Count_Args(__VA_ARGS__)) ( (void*)(_pArray), (size_t)(_nSlots), sizeof(*(_pArray)), _fnCompare, ##__VA_ARGS__)
#define  IsArraySorted( _Type, _pArray, _nSlots, ...)            csname2(IsArraySorted_, Count_Args(__VA_ARGS__)) ( (void*)(_pArray), (size_t)(_nSlots), sizeof(*(_pArray)), FNCOMPARE(_Type), ##__VA_ARGS__)
#define  IsArraySorted_0( _pArray, _nSlots, _nSizeBytes, _fnCompare)                  _IsArraySorted_c( _pArray, _nSlots, _nSizeBytes, _fnCompare, ASC )
#define  IsArraySorted_1( _pArray, _nSlots, _nSizeBytes, _fnCompare, _IsAscending)    _IsArraySorted_c( _pArray, _nSlots, _nSizeBytes, _fnCompare, _IsAscending )


static bool _IsArraySorted_c( void* p_pArray, size_t p_nSlotsInArray, size_t p_nSizeBytes, fnCompare_t fnCompare, bool p_isAscending) {
    if( p_isAscending ) {
        for( void *iter  = p_pArray + p_nSizeBytes,
                  *pStop = p_pArray + p_nSizeBytes * p_nSlotsInArray
            ; iter < pStop 
            ; iter += p_nSizeBytes )
            if( fnCompare( iter-p_nSizeBytes, iter ) > 0 )
                return false ;
    }
    else {
        for( void *iter  = p_pArray + p_nSizeBytes,
                  *pStop = p_pArray + p_nSizeBytes * p_nSlotsInArray
            ; iter < pStop 
            ; iter += p_nSizeBytes )
            if( fnCompare( iter-p_nSizeBytes, iter ) < 0 )
                return false ;
    }
    return true;
}

   // Check_Sorted(long, Array, nSize, TypeOrder) ;  _Array

#define  Check_Sorted_fn( _pArray, _nSlots, _fnCompare, ...)    csname2(Check_Sorted_, Count_Args(__VA_ARGS__)) ( (void*)(_pArray), (size_t)(_nSlots), sizeof(*(_pArray)), _fnCompare, ##__VA_ARGS__)
#define  Check_Sorted( _Type, _pArray, _nSlots, ...)            csname2(Check_Sorted_, Count_Args(__VA_ARGS__)) ( (void*)(_pArray), (size_t)(_nSlots), sizeof(*(_pArray)), FNCOMPARE(_Type) , ##__VA_ARGS__)
#define  Check_Sorted_0( _pArray, _nSlots, _nSizeBytes, _fnCompare)                  _Check_Sorted_c( _pArray, _nSlots, _nSizeBytes, _fnCompare, ASC )
#define  Check_Sorted_1( _pArray, _nSlots, _nSizeBytes, _fnCompare, _IsAscending)    _Check_Sorted_c( _pArray, _nSlots, _nSizeBytes, _fnCompare, _IsAscending )

static void  _Check_Sorted_c( void* p_pArray, size_t p_nSlotsInArray, size_t p_nSizeBytes, fnCompare_t fnCompare, bool p_isAscending) {
    char* szMethod ;
    if( p_isAscending ) 
        szMethod = gszMethodAsc ;
    else
        szMethod = gszMethodDes ;
        
    if( _IsArraySorted_c( p_pArray, p_nSlotsInArray, p_nSizeBytes, fnCompare, p_isAscending) ) 
        printf("  ->The array[%d] is in %s sorted\n", p_nSlotsInArray, szMethod );
    else 
        printf("  ->The array[%d] is not in %s sorted      ...Alert!!\n", p_nSlotsInArray, szMethod );
}

//--------------------------------------------------------------------

// ID Pattern Generators for array.
Enum(Type_List) {
    LIST_RANDOM = 0     // Generate a type list with Random Pattern.             |  .|:;|!#
  , LIST_SORTED         // Generate a type list with Sorted Ascending Pattern.   |  ..::!!||
  , LIST_REVERSED       // Generate a type list with Sorted Descending Pattern.  |  ||!!::..
  , LIST_FEWUNIQUE      // Generate a type list with few Random Uniques Pattern. |  ..|:|.:|
  , LIST_SAWTOOTH       // Generate a type list with Sawtooth Pattern.           |
  , LIST_STAGGER        // Generate a type list with Stagger Pattern.            |  .:|:.:|.
  , LIST_PLATEAU        // Generate a type list with Plateau Pattern.            |
  , LIST_SHUFFLE        // Generate a type list with Shuffle Pattern.            |
  
  , LIST_USER           // The user CUSTOM the generate list fill.
  
  ,MAX_LIST_RANDOM_TYPE // Quantity of type list 
};

// Name for Pattern Generators
char* gszRandomPattern[] = {
        "Aleatoria"         // Name for LIST_RANDOM 
      , "Ascendente"        // Name for LIST_SORTED    
      , "Descendente"       // Name for LIST_REVERSED  
      , "Aleatoria Unicos"  // Name for LIST_FEWUNIQUE 
      , "Dientes de Sierra" // Name for LIST_SAWTOOTH  
      , "Cruzado"           // Name for LIST_STAGGER   
      , "Mesetas"           // Name for LIST_PLATEAU   
      , "Shuffle"           // Name for LIST_SHUFFLE   
    } ;


Struct(ArrayPattern_t) {
    long *    pArray;
    size_t    nSlots;
    Type_List nPattern;
    char*     szPattern;
    size_t    nSeed ;
};

#define Generate_Pattern_List( _pArray, ...) csname2(Generate_Pattern_List_, Count_Args(__VA_ARGS__))( _pArray,  ##__VA_ARGS__)
#define Generate_Pattern_List_0( _pArray )                                             _Generate_Pattern_List( (_pArray), sizeofarray(_pArray), (size_t)LIST_RANDOM, UINT16_MAX, LIST_RANDOM )
#define Generate_Pattern_List_1( _pArray, _nLength)                                    _Generate_Pattern_List( (_pArray), (size_t)(_nLength),   (size_t)LIST_RANDOM, UINT16_MAX, LIST_RANDOM )
#define Generate_Pattern_List_2( _pArray, _nLength, _TypeList )                        _Generate_Pattern_List( (_pArray), (size_t)(_nLength),   (size_t)(_TypeList), UINT16_MAX, LIST_RANDOM )
#define Generate_Pattern_List_3( _pArray, _nLength, _TypeList, _nMaxValue)             _Generate_Pattern_List( (_pArray), (size_t)(_nLength),   (size_t)(_TypeList), _nMaxValue, LIST_RANDOM )
#define Generate_Pattern_List_4( _pArray, _nLength, _TypeList, _nMaxValue, _nRandSeed) _Generate_Pattern_List( (_pArray), (size_t)(_nLength),   (size_t)(_TypeList), _nMaxValue, _nRandSeed )

// Function to generate a list of random number for an array
void _Generate_Pattern_List( 
        long*  p_pArray        /* Base Array */ 
      , size_t p_nArraySize    /* Size of array */ 
      , size_t p_Type_List     /* Type of list: LIST_RANDOM, LIST_SORTED, LIST_REVERSED, 
                                                LIST_FEWUNIQUE, LIST_SAWTOOTH, LIST_STAGGER, 
                                                LIST_PLATEAU, LIST_SHUFFLE */ 
      , size_t p_nMaxValue     /* Maximun value for List. */ 
      , size_t p_nRandSeed     /* Seed for random generated */ 
) {
    int i = 0;
    Randomize(p_nRandSeed);      /* Generate a new seed for Random function. */
    switch( p_Type_List ) {
        default:
        case LIST_RANDOM:
            for( i = 0; i< p_nArraySize ; ++i )
                p_pArray[i] = Rand(p_nMaxValue);
            break;
        case LIST_SORTED:
            for( i = 0; i < p_nArraySize ; ++i )
                p_pArray[i] = i;
            break;
        case LIST_REVERSED:
            for( i = p_nArraySize-1; 0 < i ; --i )
                p_pArray[i] = i;
            p_pArray[0] = 0;
            break;
        case LIST_FEWUNIQUE: {
            int nFew = (int)p_nArraySize / 10 ;
            for( i = 0; i < p_nArraySize ; ++i )
                p_pArray[i] = nFew + Rand(nFew);
            break; }
        case LIST_SAWTOOTH:
            for( int i=0; i < p_nArraySize ; ++i )
                p_pArray[i] = i % 100;
            break;
        case LIST_STAGGER:
            for( int i=0; i < p_nArraySize ; ++i )
                p_pArray[i] = i * (1+200) % p_nArraySize ;
            break;
        case LIST_PLATEAU: {
            int nlBase = (int)Min2(p_nArraySize, p_nMaxValue) ;
            for( int i=0; i < p_nArraySize ; ++i )
                p_pArray[i] = Min2(i, nlBase) ;
            break; }
        case LIST_SHUFFLE: {
            int j = 0, k= 1 ;
            for( int i=0; i < p_nArraySize ; ++i )
                p_pArray[i] = Rand( p_nMaxValue ) ? (j += 2) : (k += 2) ;
            break; }
    }
}
    

//--------------------------------------------------------------------
 
#define show_array( _Type, _MaskType, _Array, _Slots ) \
   Procedure\
      if( !(_Slots) ) return;\
      _Type *iter= (_Array),\
            *stop= (_Array) + (_Slots) ;\
      printf("\n Array = { " _MaskType, *iter );\
      for( ++iter ; iter < stop ; ++iter ) printf(", " _MaskType, *iter);\
      printf(" }\n");\
   EndProcedure


//--------------------------------------------------------------------
//--------------------------------------------------------------------
   
#ifdef __cplusplus
    }
#endif

#endif // #ifndef __ebm_sort_h__

