/********************************************************************************************************
 * @Objetive : Provide access to THREADS to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             Also change Behavior of functions to manipulate memory; New() and Delete()
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************

 *********************************************************************************************************/
#ifndef __ebm_winthread_h__
#define __ebm_winthread_h__


#ifdef __cplusplus
    extern "C" {
#endif


// .....................................................................

Struct(Thread_t) {
    DWORD   dwId;
    HANDLE  hThread;
};

Struct(Threads_t) {
    DWORD   *adwId;
    HANDLE  *ahThread;
    uint     nThreads;
};

#define NewThread(  _ObjHandle )        Thread_t   _ObjHandle
#define NewThreadArr( _ObjArrHandle, _nThreads ) \
              Threads_t  _ObjArrHandle; \
              _ObjArrHandle.nThreads = (_nThreads) ;\
              _ObjArrHandle.adwId    = New(DWORD,  (_nThreads) ) ;\
              _ObjArrHandle.ahThread = New(HANDLE, (_nThreads) )

// .....................................................................

#define Thread(   _ObjThread, _fnFuntor, ...) csname2(Thread_, Count_Args(__VA_ARGS__))( _ObjThread, _fnFuntor, ##__VA_ARGS__)
#define Thread_0( _ObjThread, _fnFuntor)                                       Thread_4( _ObjThread, _fnFuntor, NULL, 0, 0, NULL)
#define Thread_1( _ObjThread, _fnFuntor, _PtrObjParam)                         Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, 0, 0, NULL)
#define Thread_2( _ObjThread, _fnFuntor, _PtrObjParam, _nMode)                 Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, 0, NULL)
#define Thread_3( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack)    Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, NULL)
#define Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, ptrAttrib) \
            (_ObjThread).hThread=CreateThread(   ptrAttrib \
                                              , (_nSizeStack) \
                                              , _fnFuntor \
                                              , (LPVOID)(_PtrObjParam) \
                                              , (_nMode) \
                                              , (LPVOID)(&((_ObjThread).dwId)) )

#define ThreadArr(   _ObjArrHandle, _nIndx, _fnFuntor, ...) csname2(ThreadArr_, Count_Args(__VA_ARGS__))( _ObjArrHandle, _nIndx, _fnFuntor, ##__VA_ARGS__)
#define ThreadArr_0( _ObjArrHandle, _nIndx, _fnFuntor)                                       ThreadArr_4( _ObjArrHandle, _nIndx, _fnFuntor, NULL, 0, 0, NULL)
#define ThreadArr_1( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam)                         ThreadArr_4( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam, 0, 0, NULL)
#define ThreadArr_2( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam, _nMode)                 ThreadArr_4( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam, _nMode, 0, NULL)
#define ThreadArr_3( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack)    ThreadArr_4( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, NULL)
#define ThreadArr_4( _ObjArrHandle, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, ptrAttrib) \
            (_ObjArrHandle).ahThread[(_nIndx)]=CreateThread(   ptrAttrib \
                                              , (_nSizeStack) \
                                              , _fnFuntor \
                                              , (LPVOID)(_PtrObjParam) \
                                              , (_nMode) \
                                              , (LPVOID)(&((_ObjArrHandle).adwId[(_nIndx)])) )

/* DWORD WaitThread( Thread_t oThread, int nMilisec = INFINITE ) ;
    nMilisec ::= 0          // if functions  finish, inmediatly the thread ending.
               | n          // wait 'n' mili-seconds before the finish the function, then the thread ending, where 0 < n < INFINITE.
               | INFINITE   // if function finish, the thread never ending.
   WaitThread return
        WAIT_OBJECT_0   : The function finish success.
        WAIT_TIMEOUT    : The function not finish and was forced to finish.
        WAIT_FAILED     : The function finish with error or has an error.
*/
#define WaitThread( _ObjThread, ...)  csname2(WaitThread_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define WaitThread_0( _ObjThread )                   WaitThread_1( _ObjThread, INFINITE )
#define WaitThread_1( _ObjThread, _nTimeInMilisec ) \
            WaitForSingleObject( (_ObjThread).hThread, (_nTimeInMilisec) )

#define WaitThreadArr(   _ObjThreads, ...)  csname2(WaitThreadArr_, Count_Args(__VA_ARGS__))( _ObjThreads,  ##__VA_ARGS__)
#define WaitThreadArr_0( _ObjThreads )                                WaitThreadArr_2( _ObjThreads, true, INFINITE )
#define WaitThreadArr_1( _ObjThreads,  _bWaitForAll )                 WaitThreadArr_2( _ObjThreads, _bWaitForAll, INFINITE )
#define WaitThreadArr_2( _ObjThreads,  _bWaitForAll, _nTimeInMilisec ) \
            WaitForMultipleObjects( (_ObjThreads).nThreads \
                                  , (_ObjThreads).ahThread \
                                  , (_bWaitForAll) \
                                  , (_nTimeInMilisec) )

#define DeleteThread(  _ObjThread )  CloseHandle(_ObjThread) ; _ObjThread=0
#define DeleteThreadArr( _ObjThreads ) \
            for(int i = 0 ; i < (_ObjThreads).nThreads ; ++i )\
                CloseHandle( (_ObjThreads).ahThread[i] ) ;\
            Delete( (_ObjThreads).ahThread, 0 ) ;\
            Delete( (_ObjThreads).adwId, 0 ) ;\
            (_ObjThreads).adwId = 0 ;\
            (_ObjThreads).ahThread = 0
// .....................................................................


#ifdef New
    #undef New
#endif

#define New( _Type, ... )             csname2( NewHeap_, Count_Args(__VA_ARGS__))( _Type, ##__VA_ARGS__)
#define NewHeap_0( _Type )           (_Type*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(_Type) )
#define NewHeap_1( _Type, _nSlots)   (_Type*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, (_nSlots)*sizeof(_Type) ) 
#define NewHeap_1_( _Type, _nSlots)  (_Type*)\
    Function \
        size_t nSlots = (size_t)(_nSlots) ;\
        nSlots = (nSlots)<2 ? 1 : nSlots ;\
        (size_t)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, nSlots*sizeof(_Type) );\
    EndFunction

#define NewHeap_2( \
        _Type     /* C or C++ Datatype or custom datatype. */ \
      , _nRows    /* Rows in matrix. */ \
      , _nCols    /* Columns in matrix. */ \
   ) (_Type**)\
   Function \
        size_t nRows = (_nRows),\
               nCols = (_nCols);\
        _Type** pMat = NewHeap_1(_Type*, nRows) ; \
        if( pMat ) {\
            register _Type **iter = pMat ; \
            register _Type _InitValue = (_Type)NULL;\
            for( size_t i=0 ; i < nRows ; ++i ) \
                *(iter++) = NewHeap_1( _Type, nCols ) ; \
        }\
        (ulong)pMat ; \
   EndFunction
/**/
// .....................................................................

#ifdef Delete
    #undef Delete
#endif
#define Delete( _Pointer, ... )      csname2(DeleteHeap_,Count_Args(__VA_ARGS__))(_Pointer, ##__VA_ARGS__)

#define DeleteHeap_0( _Pointer ) \
    Function \
        HeapFree( GetProcessHeap(), 0, _Pointer );\
        (long)(_Pointer = NULL); \
    EndFunction

#define DeleteHeap_1( _Array, n)\
    Function \
        HeapFree( GetProcessHeap(), 0, _Array );\
        (long)(_Array = NULL); \
    EndFunction

#define DeleteHeap_2( _Matrix, m, n) \
    Function \
        size_t _m = (size_t)(m);\
        typeof(_Matrix)    iterI = _Matrix;\
        if( _Matrix ) {\
            for( size_t i = 0 ; i < _m ; ++i, ++iterI ) \
                HeapFree( GetProcessHeap(), 0, *iterI );\
            HeapFree( GetProcessHeap(), 0, _Matrix );\
        }\
        (long)(_Matrix = NULL); \
    EndFunction
   
// .....................................................................

#ifdef __cplusplus
    }
#endif

#endif  // #ifndef __ebm_winthread_h__
