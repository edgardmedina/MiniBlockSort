/********************************************************************************************************
 * @Objetive : This library define or redefine the macros Delete() and Delete_ptr().
 *             This module is necessary beacuse the macros 'mem_delete()' and 
 *             'array_delete()' are redefined by another library and Delete() and 
 *             Delete_ptr() must be redefined.
 *             The minimun requirement is GNU C90 or GNU C++94.
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************
 *
 *********************************************************************************************************/
 
 
 // -------------------------------------------------------------------------
 #ifdef New_2
    #undef New_2
 #endif
 
#define New_2( _Type, _Rows, _Cols ) (_Type**)\
    Function\
        size_t  _nRows = (_Rows), \
                _nCols = (_Cols); \
        _Type** pMat = New_1(_Type*, _nRows) ; \
        if( pMat ) {\
            register _Type **iter = pMat ; \
            register _Type _InitValue = (_Type)NULL;\
            for( size_t i=0 ; i < _nRows ; ++i ) {\
                *(iter++) = New_1( _Type, _nCols ) ; \
                for(size_t j=0; j < _nCols ;++j) pMat[i][j] = _InitValue ;\
            }\
        } \
        (ulong)pMat ; \
    EndFunction

 // -------------------------------------------------------------------------
 
#ifdef Delete
    #undef Delete
    #undef Delete_0
    #undef Delete_1
    #undef Delete_2

    #undef Delete_ptr
    #undef Delete_ptr_0
    #undef Delete_ptr_1
    #undef Delete_ptr_2
#endif
 
// -------------------------------------------------------------------------
#define Delete( _Pointer, ... )      csname2(Delete_,Count_Args(__VA_ARGS__))(_Pointer, ##__VA_ARGS__)

#define Delete_0( _Pointer ) \
    Function \
        mem_delete( _Pointer ); \
        (long)(_Pointer = NULL); /**/\
    EndFunction

#define Delete_1( _Array, n)\
    Function \
        array_delete( _Array ); \
        (long)(_Array = NULL); /**/\
    EndFunction
//*
#define Delete_2( _Matrix, _rows, n) \
    Function \
        size_t _nRows = (size_t)(_rows);\
        typeof(_Matrix)    iterI = _Matrix;\
        if( _Matrix ) {\
            for( size_t i = 0 ; i < _nRows ; ++i, ++iterI ) \
                array_delete( *iterI ); \
            array_delete( _Matrix ); \
        }\
        (long)(_Matrix = NULL); /**/\
    EndFunction
/**
#define Delete_2( _pMatrix, _rows, n) \
    Function \
        if( _pMatrix ) {\
            size_t _nRows = (size_t)(_rows),\
                   _nBytes= sizeof( _pMatrix[0] ); \
            uchar* _pIterI = (uchar*)_pMatrix; \
            for( size_t i = 0 ; i < _nRows ; ++i, _pIterI += _nBytes ) \
                array_delete( *(uchar**)_pIterI ) ;\
            array_delete( _pMatrix );\
        }\
        (long)(_pMatrix = NULL); \
    EndFunction
/**/
// ..................................................................................

#define Delete_ptr( _Pointer, ... )      csname2(Delete_ptr_,Count_Args(__VA_ARGS__))(_Pointer, ##__VA_ARGS__)

#define Delete_ptr_0( _Pointer ) \
    Function \
        mem_delete( *(_Pointer) ); \
        mem_delete( _Pointer ); \
        (long)(_Pointer = NULL); \
    EndFunction


#define Delete_ptr_1( _pArray, _Slots)\
    Function \
        if( _pArray) { \
            size_t _nSlots = (_Slots);\
            for(int _nSlot=0 ; _nSlot < _nSlots ; ++_nSlot ) \
                mem_delete( _pArray[_nSlot] ) ;\
        } \
        array_delete( _pArray ); \
        (long)(_pArray = NULL); \
    EndFunction

// #define Delete_ptr_2( _Matrix, _Rows, _Cols) \
    // Function \
        // RecordTime_t* pRec;  \
        // if( _Matrix ) {\
            // size_t _nRows = (size_t)(_Rows),\
                   // _nCols = (size_t)(_Cols);\
            // for( size_t i = 0 ; i < _nRows ; ++i ) { \
                // for( size_t j = 0 ; j < _Cols ; ++j ) {\
                    // pRec = _Matrix[i][j] ; /**/\
                    // printf("\n   ***> Mat[%d, %d] = %d\t[%d]", i, j, pRec, _Matrix[i][j+1] ) ;\
                    // /*mem_delete(pRec);\
                    // /*printf("  method: %s", pRec->szMethod );/**/\
                    // /*mem_delete( _Matrix[i][j] ) ; /**/\
                // } \
                // array_delete( _Matrix[i] ); \
            // } \
        // }\
        // array_delete( _Matrix ); \
        // (long)(_Matrix = NULL); \
    // EndFunction
    
#define Delete_ptr_2( _Matrix, _Rows, _Cols) \
    Function \
        if( _Matrix ) {\
            size_t _nRows = (size_t)(_Rows),\
                   _nCols = (size_t)(_Cols);\
            for( size_t i = 0 ; i < _nRows ; ++i ) { \
                for( size_t j = 0 ; j < _Cols ; ++j ) \
                    mem_delete( _Matrix[i][j] ) ; \
                array_delete( _Matrix[i] ); \
            } \
        }\
        array_delete( _Matrix ); \
        (long)(_Matrix = NULL); \
    EndFunction


// ..................................................................................

