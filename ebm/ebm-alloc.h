/********************************************************************************************************
 * @Objetive : Provide generic functions to manage memory allocation works with
 *             native C or C++ compilers, using internally native function.
 *              ANSI C  : malloc(), cmalloc(), free();
 *              ANSI C++: new, new[], delete, delete[].
 *             The minimun requirement is GNU C90 or GNU C++94.
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************
 *  USER FUNCTIONS TO MANIPULATE MEMORY
 *  ===================================
 *
 *  Template Functions to Assign memory (C/C++): (All new allocated memory are initialized with zero)
 *  -------------------------------------------
 *    *<Type>  New( <Type> )                                        // Create an Object of type <Type>
 *    *<Type>  New( <Type>, size_t nSlots )                         // Create an Array with 'nSlots' slots.
 *   **<Type>  New( <Type>, size_t nRows,  size_t nCols )           // Create a Matrix with 'nRows' rows and 'nCols' columns.
 *
 *  Template Functions to Assign memory and initialización with an value (C/C++):
 *  ----------------------------------------------------------------------------
 *    *<Type>  New_set( <Type>, <Type> Init_Value )
 *    *<Type>  New_set( <Type>, size_t nItems, <Type> Init_Value )
 *   **<Type>  New_set( <Type>, size_t nRows,  size_t nCols, <Type> Init_Value )
 *
 *  Template Functions to Delete memory assign with New()  (C/C++):
 *  ---------------------------------------------------------------
 *    *<Type>  Delete( <Type> )
 *    *<Type>  Delete( <Type>, size_t nItems|NULL )
 *   **<Type>  Delete( <Type>, size_t nRows,  size_t nCols|NULL )
 *
 *  Template Functions to Delete memory assign with New() and Data es a dinamic Pointer  (C/C++):
 *  --------------------------------------------------------------------------------------------
 *    *<Type>  Delete_ptr( <Type> )
 *    *<Type>  Delete_ptr( <Type>, size_t nItems )
 *   **<Type>  Delete_ptr( <Type>, size_t nRows,  size_t nCols )
 *
 *  C-Style Functions to manipulate memory
 *  ----------------------------------------------------------------------------
 *    void* mem_allocz( size_t p_nSlots, size_t p_nSizeSlotInBytes )  // Allocate memory for one or many objects of size 'p_nSizeBytes'
 *    void* mem_alloc(  size_t p_nSlots, size_t p_nSizeSlotInBytes )  // Idem 'mem_alloc()', but initialize the memory with zeros values.
 *    void  mem_free( void* pPointer)                                   // Release memory allocated by 'mem_alloc()' or 'mem_allocz()'.

 *    void* mem_copy( <type> p_pTarget, <type> p_pSource )                          //  Template function, return a void* pointer to 'p_pTarget'.
 *    void* mem_copy_c( void* p_pTarget, const void* p_pSource, size_t p_nBytes )   //  from 'p_pSource' copy a segment of memoyy of 'p_nBytes' bytes to  'p_pTarget'
 *    void* mem_fill_c( void* p_pTarget, size_t p_nSizeInBytes, int p_nSymbol)      //  Fill an allocated memory with a initial value 'p_nSymbol' between 0 an 255.
 *
 ********************************************************************************************************/
#ifndef __alloc_h__
#define __alloc_h__

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef  EBM_Exception
    #define Check_Mem( _Ptr, _Msg) NULL     // Check out if pointer is a valid address used in "ebm\exception.h"
#endif

inline void* mem_alloc( size_t p_nSlots, size_t p_nSizeBytes ) {
   return malloc( ( p_nSlots<2 ? 1 : p_nSlots)  * p_nSizeBytes ) ;
}
inline void* mem_allocz( size_t p_nSlots, size_t p_nSizeBytes ) {
   return calloc( ( p_nSlots<2 ? 1 : p_nSlots), p_nSizeBytes ) ;
}
#define mem_free(_pVar) if(_pVar) { free(_pVar); _pVar = NULL; }


#define  mem_copy_c( _pTarget, _pSource, p_nBytes ) memcpy( (void*)_pTarget, (void*)_pSource, (size_t)p_nBytes )

#define  mem_copy_c2( _pTarget, _pSource, p_nBytes ) (void*)\
    Function\
        if( _pTarget ) {\
            register Byte *pTarget = (Byte*)_pTarget,\
                          *pSource = (Byte*)_pSource;\
            register ulong _nItems = (ulong)p_nBytes;\
            while( _nItems-- ) *pTarget++ = *pSource++;\
        }\
        (size_t)_pTarget;\
    EndFunction
inline void* __cdecl mem_copy_c1( void* p_pTarget, const void* p_pSource, size_t p_nBytes ) {
    Byte *pTarget = (Byte*)p_pTarget,
         *pSource = (Byte*)p_pSource,
         *pStop   = pSource + p_nBytes ;
    while( pSource < pStop) *pTarget++ = *pSource++;
    return p_pTarget;
}



#define mem_copy( _Target, _Source)   mem_copy_c( (void*)&(_Target), (void*)&(_Source), (size_t)sizeof(_Target) )

inline void* mem_fill_c( void* p_pTarget, size_t p_nBytes, int p_nSymbol) {
    byte *pIter = (byte*)p_pTarget,
         *pStop = pIter + p_nBytes ;
    byte nSymb = (byte)p_nSymbol;
    while( pIter < pStop) *pIter++ = nSymb;
    return p_pTarget;
}

// ..................................................................................
//  Setting the Allocate Memory Model
#define  New( _Type, ... )              csname2( New_, Count_Args(__VA_ARGS__))( _Type, ##__VA_ARGS__)

#ifdef __cplusplus
    #if __cplusplus < 200300
       #define New_0( _Type )         (_Type*)mem_fill_c( new _Type, sizeof(_Type), 0 )
       #define New_1( _Type, _nSlots) (_Type*)\
            Function \
                size_t nSlots     = (size_t)(_nSlots), \
                nSlots = nSlots < 2 ? 1 : nSlots; \
                (size_t)mem_fill_c( new _Type[ nSlots ], nSlots * sizeof(_Type), 0 ) ; \
            EndFunction
    #else
       #define New_0( _Type )         new _Type((_Type)0)
       #define New_1( _Type, _nSlots) new _Type [ (_nSlots)<2 ? 1 : (size_t)(_nSlots) ]()
   #endif
#else
    // inline register void*  _AllocateC( size_t p_nSlots, size_t p_nSizeInBytes ) {
    inline  void*  _AllocateC( size_t p_nSlots, size_t p_nSizeInBytes ) {
        return calloc( (p_nSlots < 2 ? 1 : p_nSlots) , p_nSizeInBytes ) ;
    }

   #define New_0( _Type )            (_Type*)calloc( 1,  sizeof(_Type) )
   #define New_1( _Type, _nSlots)    (_Type*)_AllocateC( (_nSlots), sizeof(_Type) )
#endif

#define New_2( _Type, _nRows, _nCols)

// ..................................................................................

#define  New_set( _Type, _InitrValue, ... )              csname2( New_set_, Count_Args(__VA_ARGS__))( _Type, _InitrValue, ##__VA_ARGS__)
#ifdef __cplusplus
    #define New_set_0( _Type, _InitrValue )               new _Type ((_InitrValue))
    #define New_set_1( _Type, _nSlots, _InitValue)        new _Type [ (_nSlots)<2 ? 1 : (size_t)(_nSlots) ] ((_InitValue))
#else
    #define New_set_0( _Type, _InitrValue )               (_Type*)mem_copy_c( malloc( sizeof(_Type) ), (void*)&(_Type)(_InitValue), sizeof(_Type) )
    #define New_set_1( _Type, _InitrValue, _nSlots)       (_Type*)New_Array_Init( (_nSlots), (void*)&(_Type)(_InitValue), sizeof(_Type) )

    void* New_Array_Init( size_t p_nSlots, void* p_pInitValue, size_t p_nSizeBytes )  {
        p_nSlots = p_nSlots<2 ? 1 : p_nSlots ;
        size_t nStop = p_nSlots * p_nSizeBytes ;
        char *pAlloc = (char*) malloc( nStop ),
             *pIter   = pAlloc,
             *pStop   = pAlloc +  nStop ;
        while( pIter < pStop ) {
            mem_copy_c( pIter, p_pInitValue, p_nSizeBytes ) ;
            pIter += p_nSizeBytes;
        }
        return (void*)pAlloc;
    }

#endif

// ..................................................................................
//  Setting the Free Memory Model
#ifdef __cplusplus
   #define   mem_delete( _pPointer ) if( (_pPointer) ) delete (_pPointer)
   #define array_delete( _pArray )   if( (_pArray) )   delete[] (_pArray)
#else
   #define   mem_delete( _pPointer ) if( (_pPointer) ) free(_pPointer)
   #define array_delete( _pArray )   if( (_pArray)   ) free(_pArray)
#endif
// ..................................................................................

// Call this library to define Delete() and Delete_ptr() Functions Macros.
#include "ebm-alloc-aux.h"

#ifdef __cplusplus
    }
#endif

#endif // end #define __alloc_h__
