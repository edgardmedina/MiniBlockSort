/********************************************************************************************************
 * @Objetive : Provide access to THREADS to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             Also change Behavior of functions to manipulate memory; New() and Delete()
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************/
#ifndef __ebm_thread_nothing_h__
#define __ebm_thread_nothing_h__


#ifdef __cplusplus
    extern "C" {
#endif


/* .....................................................................
 * First of all, is necesary Redefine  New() and Delete()
 * for Allocate memory in Thread Heap Model Memory.
 *....................................................................*/


// .....................................................................
typedef ulong  Return_t ;
typedef Return_t (*ThreadFunction_t)( void* ) ;


Struct(Thread_t) {
    DWORD   dwId;           // Identifier for thread.
    HANDLE  hThread;        // Handle for Thread.
    DWORD   nReturn;        // Exit code for Thread.
    DWORD   nResult;        // Code for normal terminate of thread  in Wait_Thread().
    BOOL    bGetExitCode;   // Boolean for get exit code from thread
};

Struct(Threads_t) {
    size_t  nThreads;       // Quantity of threads
    HANDLE *ahThread;       // Array of Handle for threads.
    DWORD  *anId,           // Array of Identifier for threads.
           *anReturn;       // Array of Code for normal terminate of thread.
    BOOL   *abGetExitCode;  // Boolean for get exit code from thread
    DWORD   nResult;        // Code for normal terminate of thread in Wait_Threads().
};

// .....................................................................
#define Get_ProcessesId()   0
#define Get_Processes()     0
#define Get_Thread()        0
// .....................................................................

#define Return(...)                     csname2(Return_, Count_Args(__VA_ARGS__))( __VA_ARGS__ )
#define Return_0()                      Return_1( 0 )
#define Return_1( _nExitCode )          return (ulong)(_nExitCode)

#define Exit_Thread(...)                
#define Exit_Thread_0()                 
#define Exit_Thread_1( _nExitCode )     

#define Terminate_Thread( _ObjThread, _param, ... )  csname2(Terminate_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, _param,  ##__VA_ARGS__)
#define Terminate_Thread_0( _ObjThread, _nExitCode )
#define Terminate_Thread_1( _ObjGroupThread, _nIndex, _nExitCode )

// .....................................................................
// Functions to get information
// .....................................................................

#define Exit_Code                     Returned
#define Returned(   _ObjThread, ...)  csname2(Returned_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Returned_0( _ObjThread )            ((_ObjThread).nReturn)
#define Returned_1( _ObjThreads, _nIndx )   ((_ObjThreads).anReturn[_nIndx])

#define Id_Thread(   _ObjThread, ...)  csname2(Id_Thread_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Id_Thread_0( _ObjThread )            ((_ObjThread).dwId)
#define Id_Thread_1( _ObjThreads, _nIndx )   ((_ObjThreads).anId[_nIndx])

#define Handler_Thread(   _ObjThread, ...)  csname2(Handler_Thread_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Handler_Thread_0( _ObjThread )            ((_ObjThread).hThread)
#define Handler_Thread_1( _ObjThreads, _nIndx )   ((_ObjThreads).ahThread[_nIndx])

#define Returned_Success(   _ObjThread, ...)  csname2(Returned_Success_, Count_Args(__VA_ARGS__))( _ObjThread,  ##__VA_ARGS__)
#define Returned_Success_0( _ObjThread )            ((_ObjThread).bGetExitCode)
#define Returned_Success_1( _ObjThreads, _nIndx )   ((_ObjThreads).abGetExitCode[_nIndx])

// .....................................................................

inline register void _Init_ObjThread_0( Thread_t* p_pObjThread ) {
    p_pObjThread->hThread = NULL ,
    p_pObjThread->dwId    = 0 ,
    p_pObjThread->nReturn = 0 ,
    p_pObjThread->nResult = 0 ;
    p_pObjThread->bGetExitCode = FALSE ;
}

inline register void _Init_ObjThread_1( Threads_t* p_pObjGroupThread, size_t p_nThreads ) {
    p_nThreads = 0 < p_nThreads ? p_nThreads : 1  ;
    p_pObjGroupThread->nThreads      = p_nThreads ;
    p_pObjGroupThread->anId          = New(DWORD,  p_nThreads ) ;
    p_pObjGroupThread->anReturn      = New(DWORD,  p_nThreads ) ;
    p_pObjGroupThread->ahThread      = New(HANDLE, p_nThreads ) ;
    p_pObjGroupThread->abGetExitCode = New(BOOL,   p_nThreads ) ;
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
        p_pObjThread->dwId    = 0 ,
        p_pObjThread->nReturn = 0 ,
        p_pObjThread->nResult = 0 ;
        p_pObjThread->hThread = NULL;
        p_pObjThread->bGetExitCode = TRUE ;
}

#define Close_Threads( _ObjThreads )    _Close_Threads( &(_ObjThreads) )
inline register void
_Close_Threads( Threads_t* p_pObjGroupThread ) {

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
    Procedure\
            (_ObjThread).hThread=(HANDLE)_fnFuntor;\
            (_ObjThread).nReturn=_fnFuntor(  (LPVOID)(_PtrObjParam)  ); \
    EndProcedure

#define New_Threads(  _ObjGroupThread, _nIndx, _fnFuntor, ...) csname2(New_Threads_, Count_Args(__VA_ARGS__))(_ObjGroupThread, _nIndx, _fnFuntor, ##__VA_ARGS__)
#define New_Threads_0( _ObjGroupThread, _nIndx, _fnFuntor)                                       New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, NULL, 0, 0, NULL)
#define New_Threads_1( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam)                         New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, 0, 0, NULL)
#define New_Threads_2( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode)                 New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, 0, NULL)
#define New_Threads_3( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack)    New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, NULL)
#define New_Threads_4( _ObjGroupThread, _nIndx, _fnFuntor, _PtrObjParam, _nMode, _nSizeStack, ptrAttrib) \
    Procedure\
            (_ObjGroupThread).ahThread[(_nIndx)]=(HANDLE)_fnFuntor;\
            (_ObjGroupThread).anReturn[(_nIndx)]= _fnFuntor( (LPVOID)(_PtrObjParam) );\
    EndProcedure

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
#define Wait_Thread_0( _ObjThread )                   Wait_Thread_1( _ObjThread, INFINITE )
#define Wait_Thread_1( _ObjThread, _nTimeInMilisec )  ( (_ObjThread).nResult = 0 )

inline register BOOL
_GetExitCodeThreads( Threads_t* p_pObjThreads ) {
    return p_pObjThreads->nResult ;
}
#define Wait_Threads(   _ObjThreads, ...)  csname2(Wait_Threads_, Count_Args(__VA_ARGS__))( _ObjThreads,  ##__VA_ARGS__)
#define Wait_Threads_0( _ObjThreads )                  Wait_Threads_2( _ObjThreads, true, INFINITE )
#define Wait_Threads_1( _ObjThreads,  _bWaitForAll )   Wait_Threads_2( _ObjThreads, _bWaitForAll, INFINITE )
#define Wait_Threads_2( _ObjThreads,  _bWaitForAll, _nTimeInMilisec ) \
    (  (_ObjThreads).nResult = 0\
     , _GetExitCodeThreads( &_ObjThreads ) \
    )

#define Suspend_Thread( _ObjThread, ...)   csname2(Suspend_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, ##__VA_ARGS__)
#define Suspend_Thread_0( _ObjThread )
#define Suspend_Thread_1( _ObjThreads, _nIdxObject )

#define Resume_Thread(   _ObjThread, ...)   csname2(Resume_Thread_, Count_Args(__VA_ARGS__))( _ObjThread, ##__VA_ARGS__)
#define Resume_Thread_0( _ObjThread )
#define Resume_Thread_1( _ObjThreads, _nIdxObject )

// .....................................................................

#ifdef __cplusplus
    }
#endif

#endif  // #ifndef __ebm_thread_nothing_h__
