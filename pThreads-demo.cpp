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


#include<pthread.h>
#include<pthread.h>
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
        static void *   ThdFn(void *pObj);
        int             Suspend(void);
        int             Resume(char (*)(void*), void *);
        int             IsSuspended();
        int             GetErr();
        unsigned long   GetThreadID();
        int             Quit();
        int             TerminateSysThread();

        // Constructer / Destructer
        CSysThread();
        virtual ~CSysThread();
};





__________________SysThread.cpp____________________________________

#include<iostream.h>
#include<unistd.h>
#include "SysThread.h"
//_____________________________________________________________________________________________
//_________________________________CONSTRUCTER_________________________________________________
CSysThread::CSysThread() {
    int nRet;

    fnFunction = (char (*)(void*))Dummy;
    m_pObjThdFn    = NULL;
    m_nState       = QUITTED;
    m_nSuspend     = 0;
    m_hTread        = 0;
    m_nErr         = 0;

    nRet = pthread_mutex_init(&m_oSuspendMutex,NULL);	// Return Check
    if(nRet != 0) {
        m_nErr = 1;
        return;
    }

    nRet = pthread_cond_init(&m_oResumeCond,NULL);	// Return Check
    if(nRet != 0) {
        m_nErr = 1;
        return;
    }


    nRet = pthread_create(&m_hTread, NULL, ThdFn,this);
    if(nRet != 0) {
        m_nErr = 1;
        return;
    }
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
int CSysThread::Resume(char (*func)(void*), void *pObj) {
    int nCondRet;

    if( IsThdSuspended() == false) 
        return -1;

    m_pObjThdFn      = pObj;
    fnFunction = (char (*)(void*))func;
    nCondRet         = pthread_cond_signal(&m_oResumeCond);
    if( nCondRet != 0 ) 
        return -1;
    return 1;
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
int CSysThread::Quit() {
    if(m_hTread == 0) 
        return -1;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    int nRet = pthread_cancel(m_hTread);
    m_nState = EXITED;
    return nRet;
}

//_____________________________________________________________________________________________
//_____________________________________________ThdFn___________________________________________

void* CSysThread::ThdFn(void *ptr) {
    CSysThread *pCthd = (CSysThread*)ptr;
    pthread_cleanup_push( &CallBack, pCthd);
    char ch;
    int nSuspRet;


    while(1) {
        if(pCthd->m_nSuspend==1) {
            pCthd->m_nSuspend = 0;
            pCthd->m_nState = SUSPENDED;
            pthread_cond_wait(&pCthd->m_oResumeCond,&pCthd->m_oSuspendMutex);
        }
        pCthd->m_nState = RESUMED;
        usleep(2000);
        // check if the funtion is exited, Call the thread cancelation
        ch = (*(pCthd->fnFunction))(pCthd->m_pObjThdFn);
        if(ch < 0) {
            nSuspRet = pCthd->Suspend();
            if(nSuspRet < 0) {
                pthread_exit(0);
                pCthd->m_nState = EXITED;
            }
        }
    }
}

//_____________________________________________________________________________________________
//__________________________________________CallBack___________________________________________

void CSysThread::CallBack(void *ptr) {
    CSysThread *pCthd = (CSysThread*)ptr;

    pCthd->m_nState = QUITTED;
    pCthd->m_hTread  = 0;
}
//_____________________________________________________________________________________________
//__________________________________________CallBack___________________________________________
char CSysThread::Dummy(void *arg) {
    sleep(1);
    return 1;
}


// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//**************************************************************
