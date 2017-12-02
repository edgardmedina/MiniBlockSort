/********************************************************************************************************
 * @Objetive : Provide access to THREADS to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             Also change Behavior of functions to manipulate memory; New() and Delete()
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************/
#ifndef __ebm_thread_unix_h__
#define __ebm_thread_unix_h__


#ifdef __cplusplus
    extern "C" {
#endif


/* .....................................................................
 * First of all, is necesary Redefine  New() and Delete()
 * for Allocate memory in Thread Heap Model Memory.
 *....................................................................*/
/*
inline register void*  _HeapAllocate( size_t p_nSlots, size_t p_nSizeInBytes ) {
    return (void*)HeapAlloc(  GetProcessHeap()
                            , HEAP_ZERO_MEMORY
                            , (p_nSlots < 2 ? 1 : p_nSlots) * p_nSizeInBytes
                           ) ;
}

#ifdef New_0
    #undef New_0
    #undef New_1
#endif

#define New_0( _Type )         (_Type*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(_Type) )
#define New_1( _Type, _nSlots) (_Type*)_HeapAllocate( (_nSlots), sizeof(_Type) )


#ifndef __cplusplus
    #if defined(mem_delete)
        #undef mem_delete
        #undef array_delete
    #endif

    #define   mem_delete( _pPointer ) if( (_pPointer) ) HeapFree( GetProcessHeap(), HEAP_NO_SERIALIZE, (LPVOID)(_pPointer) )
    #define array_delete( _pArray )   if( (_pArray) )   HeapFree( GetProcessHeap(), HEAP_NO_SERIALIZE, (LPVOID)(_pArray) )

    // Call this library to redefine Delete() and Delete_ptr() Functions Macros.
    #include "ebm-alloc-aux.h"
#endif
/**/
// .....................................................................
enum {
    SUSPENDED = 1
  , EXITED	
  , RESUMED	
  , QUITTED	
} ;

typedef void*  Return_t ;
typedef Return_t (*ThreadFunction_t)( void* ) ;

Return_t _fnDummy(void* p_pParam) {
    return 1;
}
        
Struct(MutexLock_t) {
    pthread_t         hThread ;
    pthread_mutex_t	  omSuspendMutex;
    pthread_cond_t	  ocResumeCond; 
    ThreadFunction_t  fnFunction ;
    void             *pObjThdFn;
    int               nSuspend ;
    int               nErr ;
    pCthd->m_nState = QUITTED;
}
Struct(Thread_t) {
    pthread_mutex_t  m_oSuspendMutex;
    pthread_cond_t   m_oResumeCond; 
    pthread_t        m_hTread;
    void            *m_pObjThdFn;
    int              m_nState;
    int              m_nSuspend;
    ThreadFunction_t fnFunction;
    ThreadFunction_t fnDummy;
    
    int             m_nErr;
}
Return_t        CallBack(void *arg);
bool            IsThdSuspended();

Struct(Thread_t) {
    pthread_t    hThread;        // Handle for Thread.
    int          nStatus;        // Error status for created thread.
    void*        pReturn;        // Returnned value for Thread.
    bool         bGetExitCode;   // Boolean for get exit code from thread
    DWORD        nResult;        // Code for normal terminate of thread  in Wait_Thread().
    MutexLock_t  mutLock;        // Auxiliar structure for suspend a thread.
};

Struct(Threads_t) {
    size_t       nThreads;       // Quantity of threads
    pthread_t   *ahThread;       // Array of Handle for threads.
    int         *anStatus;       // Array of Identifier for threads.
    void*       *apReturn;       // Array of Code Returned for normal terminate of thread.
    bool        *abGetExitCode;  // Boolean for get exit code from thread
    MutexLock_t *amutLock;       // Auxiliar structure array for suspend a thread.
    DWORD        nResult;        // Code for normal terminate of thread in Wait_Threads().
};

// .....................................................................

#define Return                         Return_Thread
#define Return_Thread(...)             csname2(Return_Thread_, Count_Args(__VA_ARGS__))( __VA_ARGS__ )
#define Return_Thread_0()              pthread_exit(void*)NULL )
#define Return_Thread_1( _nExitCode )  pthread_exit( (void*)(_nExitCode) )

#define Terminate_Thread( _ObjThread, _param, ... )  csname2(Terminate_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, _param,  ##__VA_ARGS__)
#define Terminate_Thread_0( _ObjThread, _nExitCode )               TerminateThread( (_ObjThread).hThread, (DWORD)(_nExitCode) )
#define Terminate_Thread_1( _ObjGroupThread, _nIndex, _nExitCode ) TerminateThread( (_ObjGroupThread).ahThread[_nIndex], (DWORD)(_nExitCode) )

// .....................................................................
// Functions to get information
// .....................................................................

#define Exit_Code                     Returned
#define Returned(   _ObjThread, ...)  csname2(Returned_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Returned_0( _ObjThread )            ((size_t)((_ObjThread).pReturn))
#define Returned_1( _ObjThreads, _nIndx )   ((size_t)((_ObjThreads).apReturn[_nIndx]))

#define Id_Thread(   _ObjThread, ...)  csname2(Id_Thread_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Id_Thread_0( _ObjThread )            ((_ObjThread).nStatus)
#define Id_Thread_1( _ObjThreads, _nIndx )   ((_ObjThreads).anStatus[_nIndx])

#define Handle_Thread(   _ObjThread, ...)  csname2(Handle_Thread_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Handle_Thread_0( _ObjThread )            ((_ObjThread).hThread)
#define Handle_Thread_1( _ObjThreads, _nIndx )   ((_ObjThreads).ahThread[_nIndx])

#define Returned_Success(   _ObjThread, ...)  csname2(Returned_Success_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Returned_Success_0( _ObjThread )            ((_ObjThread).bGetExitCode)
#define Returned_Success_1( _ObjThreads, _nIndx )   ((_ObjThreads).abGetExitCode[_nIndx])

// .....................................................................

inline register void _Init_ObjThread_0( Thread_t* p_pObjThread ) {
    p_pObjThread->hThread = NULL ,
    p_pObjThread->nStatus = 0 ,
    p_pObjThread->nReturn = 0 ,
    p_pObjThread->nResult = 0 ;
    p_pObjThread->bGetExitCode = FALSE ;
    
    MutexLock_t *mutLock = &(p_pObjThread->MutexLock_t);
    mutLock->omSuspendMutex = (pthread_mutex_t)0;
    mutLock->ocResumeCond   = (pthread_cond_t)0; 
    mutLock->fnFunction     = _fnDummy;
    mutLock->pObjThdFn      = NULL;
    mutLock->nSuspend       = false ;

    mutLock->fnThreadFunction = (char (*)(void*))Dummy;
    mutLock->m_pObjThdFn    = NULL;
    mutLock->m_nState       = QUITTED;
    mutLock->m_nSuspend     = 0;
    mutLock->m_ThdID        = 0;
    mutLock->nErr           = 0;

    p_pObjThread->pthread_mutex_t  m_oSuspendMutex;
    p_pObjThread->pthread_cond_t   m_oResumeCond; 
    p_pObjThread->pthread_t        m_hTread;
    p_pObjThread->void            *m_pObjThdFn;
    p_pObjThread->int              m_nState;
    p_pObjThread->int              m_nSuspend;
    p_pObjThread->ThreadFunction_t fnFunction;
    p_pObjThread->ThreadFunction_t fnDummy;
    p_pObjThread->int              m_nErr;
}

inline register void _Init_ObjThread_1( Threads_t* p_pObjGroupThread, size_t p_nThreads ) {
    p_nThreads = 0 < p_nThreads ? p_nThreads : 1  ;
    p_pObjGroupThread->nThreads      = p_nThreads ;
    p_pObjGroupThread->ahThread      = New(pthread_t, p_nThreads ) ;
    p_pObjGroupThread->anStatus      = New(int,       p_nThreads ) ;
    p_pObjGroupThread->apReturn      = New(void*,     p_nThreads ) ;
    p_pObjGroupThread->abGetExitCode = New(bool,      p_nThreads ) ;
}

#define Thread(   _ObjThread, ... )          csname2(Thread_, Count_Args(__VA_ARGS__))( _ObjThread, ##__VA_ARGS__)
#define Thread_0( _ObjThread ) \
            Thread_t   _ObjThread ;\
            _Init_ObjThread_0( &(_ObjThread) )

#define Thread_1( _ObjGroupThread, _nThreads ) \
            Threads_t  _ObjGroupThread; \
            _Init_ObjThread_1( &(_ObjGroupThread), (_nThreads) )

#define ReThread(  _ObjThread, ... )  csname2(ReThread_, Count_Args(__VA_ARGS__))( _ObjThread, ##__VA_ARGS__)
#define ReThread_0( _ObjThread ) \
    Procedure \
        _Close_Thread( &(_ObjThread) ) ; \
        _Init_ObjThread_0( &(_ObjThread) ) ; \
    EndProcedure
#define ReThread_1( _ObjGroupThread, _nThreads ) \
    Procedure \
        _Close_Threads( &(_ObjGroupThread) ) ; \
        _Init_ObjThread_1( &(_ObjGroupThread), (_nThreads) ) :\
    EndProcedure

#define Close_Thread(  _ObjThread )  _Close_Thread( &(_ObjThread) )
inline register void
_Close_Thread(  Thread_t* p_pObjThread )  {
    /*
    if( p_pObjThread->hThread )
        CloseHandle( p_pObjThread->hThread ) ;
    /**/
    p_pObjThread->hThread = NULL;
    p_pObjThread->nStatus = 0 ,
    p_pObjThread->pReturn = NULL ,
    p_pObjThread->nResult = 0 ;
    p_pObjThread->bGetExitCode = true ;
}

#define Close_Threads( _ObjThreads )    _Close_Threads( &(_ObjThreads) )
inline register void
_Close_Threads( Threads_t* p_pObjGroupThread ) {
    /*
    DWORD nThreads = p_pObjGroupThread->nThreads ;
    for(int i = 0 ; i < nThreads ; ++i )
        CloseHandle( p_pObjGroupThread->ahThread[i] ) ;
    /**/
    Delete( p_pObjGroupThread->anId, 0 ) ;
    Delete( p_pObjGroupThread->anReturn, 0 ) ;
    Delete( p_pObjGroupThread->ahThread, 0 ) ;
    Delete( p_pObjGroupThread->abGetExitCode, 0 ) ;

    p_pObjGroupThread->anId     = NULL ,
    p_pObjGroupThread->anReturn = NULL ,
    p_pObjGroupThread->ahThread = NULL ,
    p_pObjGroupThread->abGetExitCode = NULL ;
}
    
// .....................................................................

#define New_Thread(   _ObjThread, _fnFuntor, ...) csname2(New_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, _fnFuntor, ##__VA_ARGS__)
#define New_Thread_0( _ObjThread, _fnFuntor)                                       New_Thread_4( _ObjThread, _fnFuntor, NULL, 0, 0, NULL)
#define New_Thread_1( _ObjThread, _fnFuntor, _PtrObjParam)                         New_Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, 0, 0, NULL)
#define New_Thread_2( _ObjThread, _fnFuntor, _PtrObjParam, _nMode)                 New_Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, 0, NULL)
#define New_Thread_3( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack)    New_Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, NULL)
#define New_Thread_4( _ObjThread, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, ptrAttrib) \
            (_ObjThread).nStatus = pthread_create(  &((_ObjThread).hThread) \
                                                  , NULL \
                                                  , _fnFuntor \
                                                  , (void*)(_PtrObjParam) \
                                                 )

#define New_Threads(  _ObjGroupThread, _nIndx, _fnFuntor, ...) csname2(New_Threads_, Count_Args(__VA_ARGS__))(_ObjGroupThread, _nIndx, _fnFuntor, ##__VA_ARGS__)
#define New_Threads_0( _ObjGroupThread, _nIndx, _fnFuntor)                                       New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, NULL, 0, 0, NULL)
#define New_Threads_1( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam)                         New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, 0, 0, NULL)
#define New_Threads_2( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode)                 New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, 0, NULL)
#define New_Threads_3( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack)    New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, NULL)
#define New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, ptrAttrib) \
            (_ObjGroupThread).anStatus[_nIndx] = pthread_create(  &((_ObjGroupThread).ahThread[_nIndx]) \
                                                                , NULL \
                                                                , _fnFuntor \
                                                                , (void*)(_PtrObjParam) \
                                                               )

/* DWORD Wait_Thread( Thread_t oThread, int nMilisec = INFINITE ) ;
    nMilisec ::= 0          // if functions  finish, inmediatly the thread ending.
               | n          // wait 'n' mili-seconds before the finish the function, then the thread ending, where 0 < n < INFINITE.
               | INFINITE   // if function finish, the thread never ending.
   Wait_Thread return
        WAIT_OBJECT_0   : The function finish success.
        WAIT_TIMEOUT    : The function not finish and was forced to finish.
        WAIT_FAILED     : The function finish with error or has an error.
*/
#define Wait_Thread( _ObjThread, ...)  csname2(Wait_Thread_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Wait_Thread_0( _ObjThread )                 Wait_Thread_1( _ObjThread, INFINITE )
#define Wait_Thread_1( _ObjThread, _nTimeInMilisec ) \
    (  (_ObjThread).nResult = pthread_join( (_ObjThread).hThread, &((_ObjThread).pReturn) ) )

#define Wait_Threads(   _ObjThreads, ...)  csname2(Wait_Threads_, Count_Args(__VA_ARGS__))( _ObjThreads,  ##__VA_ARGS__)
#define Wait_Threads_0( _ObjThreads )                  Wait_Threads_2( _ObjThreads, true, INFINITE )
#define Wait_Threads_1( _ObjThreads,  _bWaitForAll )   Wait_Threads_2( _ObjThreads, _bWaitForAll, INFINITE )
#define Wait_Threads_2( _ObjThreads,  _bWaitForAll, _nTimeInMilisec ) \
    for( int i=0; i < (_ObjThreads).nThreads ; ++i ) \
          (_ObjThread).nResult = pthread_join( (_ObjThread).ahThread[i], &((_ObjThread).apReturn[i]) ) 

// .....................................................................

bool _IsThdSuspended( MutexLock_t* p_oMutLock ) {
    if( pthread_mutex_trylock( &(p_oMutLock->omSuspendMutex)) ) 
        return false;

    // Thread was suspended
    pthread_mutex_unlock( &(p_oMutLock->omSuspendMutex) );
    return true;
}


int _Pthread_Suspend( MutexLock_t* p_oMutLock ) {
    if( _IsThdSuspended( p_oMutLock ) ) 
        return 0;	// Alredy Suspended

    p_oMutLock->nSuspend = 1;
    return 1;
}

int _Pthread_Resume( MutexLock_t* p_oMutLock
                    , char (*func)(void*)
                    , void *pObj
                   ) {
    if( _IsThdSuspended( p_oMutLock ) == false) 
        return -1;

    m_pObjThdFn    = pObj;
    fnFunction = (char (*)(void*))func;
    
    return (pthread_cond_signal( &(p_oMutLock->ocResumeCond) ) ? -1 : 1 ) ;
}
          
#define Suspend_Thread( _ObjThread, ...)   csname2(Suspend_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, ##__VA_ARGS__)
#define Suspend_Thread_0( _ObjThread )               pthread_suspend( (_ObjThread).hThread )
#define Suspend_Thread_1( _ObjThreads, _nIdxObject ) pthread_suspend( (_ObjThreads),ahThread[_nIdxObject] )

#define Resume_Thread(   _ObjThread, ...)   csname2(Resume_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, ##__VA_ARGS__)
#define Resume_Thread_0( _ObjThread )               pthread_continue( (_ObjThread).hThread )
#define Resume_Thread_1( _ObjThreads, _nIdxObject ) pthread_continue( (_ObjThreads),ahThread[_nIdxObject] )

// .....................................................................

#ifdef __cplusplus
    }
#endif

#endif  // #ifndef __ebm_thread_unix_h__
