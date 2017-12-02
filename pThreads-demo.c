/*________________________________SysThread.h_________________________
from url:
  https://www.linuxquestions.org/questions/programming-9/resume-and-suspend-pthreads-184535/
  
  
    I am including the cpp and header file with this messege, you can make a 
    (.a) file, i mean library and use these functions, directly. If you need 
    to plz revise the Quit mechanism as it is not that good, due to short time 
    i could not spare more time on it. The thread is created with a Dummy 
    function maped and suspended, then you can resume the thread, and can give 
    the your function (static function), as a parameter to resume function with 
    the refference of class.  
*/


#include<unistd.h>
#include<pthread.h>
#include<iostream.h>

enum {
    SUSPENDED = 1
  , EXITED	
  , RESUMED	
  , QUITTED	
} ;

class CSysThread {
    private:
        pthread_mutex_t	m_oSuspendMutex;
        pthread_cond_t	m_oResumeCond; 
        int             m_nSuspend;
        int             m_nState;
        pthread_t       m_hTread;
        static char     Dummy(void *arg);
        char          (*fnFunction)(void *);
        void           *m_pObjThdFn;
        static void     CallBack(void *arg);
        bool            IsThdSuspended();

    protected:
        int m_nErr;

    public:
        ThreadFunction_t  fnCallFunction;
        int               Suspend(void);
        int               Resume(char (*)(void*), void *);
        int               IsSuspended();
        int               GetErr();
        unsigned long     GetThreadID();
        int               Quit();
        int               TerminateSysThread();

        // Constructer / Destructer
        CSysThread();
        virtual ~CSysThread();
};


//__________________SysThread.cpp____________________________________

#include "SysThread.h"
//_____________________________________________________________________________________________
//_________________________________CONSTRUCTER_________________________________________________
CSysThread::CSysThread() {
    int nRet;

    fnFunction   = fnDummy;
    m_pObjThdFn  = NULL;
    
    m_nState     = QUITTED;
    m_nSuspend   = 0;
    m_hTread     = 0;
    m_nErr       = 0;

    nRet = pthread_mutex_init(&m_oSuspendMutex,NULL);	// Return Check
    if(nRet != 0) 
        { m_nErr = 1; return; }

    nRet = pthread_cond_init(&m_oResumeCond,NULL);	// Return Check
    if(nRet != 0) 
        { m_nErr = 1; return; }

    nRet = pthread_create(&m_hTread, NULL, fnCallFunction, this );
    if(nRet != 0) 
        { m_nErr = 1; return; }
        
    m_nState = RESUMED;
    nRet = pthread_mutex_lock(&m_oSuspendMutex);	// Return Check
    if(nRet != 0) {
        m_nErr = 1;
        return;
    }

    Suspend();
}

//_____________________________________________________________________________________________
//_________________________________DESTRUCTER__________________________________________________
CSysThread::~CSysThread() {
    if(m_hTread == 0) return;
    pthread_exit(0);
    m_nState = EXITED;
}

//_____________________________________________________________________________________________
//_____________________________________SUSPEND_________________________________________________
int CSysThread::Suspend() {
    if( IsThdSuspended() ) return 0;	// Alredy Suspended

    m_nSuspend = 1;
    return 1;
}
//_____________________________________________________________________________________________
//______________________________________RESUME_________________________________________________
int CSysThread::Resume(ThreadFunction_t func, void *pObj) {
    if( !IsThdSuspended() ) 
        return -1;

    m_pObjThdFn = pObj;
    fnFunction  = func ;
    return  pthread_cond_signal(&m_oResumeCond) ? -1 : 1;
}
//_____________________________________________________________________________________________
//______________________________________IsSuspended____________________________________________
bool CSysThread::IsThdSuspended() {
    if( pthread_mutex_trylock(&m_oSuspendMutex) ) 
        return false;

    // Thread was suspended
    pthread_mutex_unlock(&m_oSuspendMutex);
    return true;
}

int CSysThread::IsSuspended() {
    return m_nState;
}

//_____________________________________________________________________________________________
//_________________________________________GetErr______________________________________________
int CSysThread::GetErr() {
    return m_nErr;
}
//_____________________________________________________________________________________________
//_________________________________________GetThreadID______________________________________________
unsigned long CSysThread::GetThreadID() {
    return m_hTread;
}
//_____________________________________________________________________________________________
//_________________________________________TerminateSysThread__________________________________
int CSysThread::TerminateSysThread() {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    pthread_cancel(m_hTread);
    m_nState = EXITED;
    return 1;
}
//_____________________________________________________________________________________________
//_________________________________________Quit________________________________________________
int CSysThread::Quit(Thread_t* p_pThread) {
    if(p_pThread->m_hTread == 0) 
        return -1;
    pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    int nRet = pthread_cancel( p_pThread->m_hTread );
    p_pThread->m_nState = EXITED;
    return nRet;
}

//_____________________________________________________________________________________________
//_____________________________________________ThdFn___________________________________________

void* CSysThread::fnCallFunction(void *ptr) {
    Thread_t* pThread = (Thread_t*)ptr;
    pthread_cleanup_push( &CallBack, ptr);

    char ch;
    int nSuspRet;

    while(1) {
        if( pThread->m_nSuspend == 1 ) {
            pThread->m_nSuspend = 0;
            pThread->m_nState = SUSPENDED;
            pthread_cond_wait( &pThread->m_oResumeCond,
                               &pThread->m_oSuspendMutex );
        }
        pThread->m_nState = RESUMED;
        usleep(2000);
        // check if the funtion is exited, Call the thread cancelation
        ch = (pThread->fnFunction)(pThread->m_pObjThdFn) ;
        if( ch < 0 ) {
            nSuspRet = pThread->Suspend();
            if( nSuspRet < 0 ) {
                pthread_exit(0);
                pThread->m_nState = EXITED;
            }
        }
    }
}

//_____________________________________________________________________________________________
//__________________________________________CallBack___________________________________________

void CSysThread::CallBack(void * p_pArg) {
    Thread_t* pThread = (Thread_t*)p_pArg;

    pThread->m_nState = QUITTED;
    pThread->m_hTread  = 0;
}
//_____________________________________________________________________________________________
//__________________________________________CallBack___________________________________________
char CSysThread::Dummy(void *arg) {
    sleep(1);
    return 1;
}


// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//**************************************************************
