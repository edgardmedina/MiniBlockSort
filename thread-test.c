/********************************************************************************************************
 * @Objetive : Test the use of thread in library ebm.h (only over MS Windows or Wine on Linux )
 *             it use ISO C99
 * @version  : v1.0, Copyleft Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************/

// This macro definition activate the threats in "ebm.h" (Only over Windows or Wine on Linux)
#define EBM_THREAD          // Uncomment, Activate THREADS; Comment, deactiva THREADS

#include "ebm\ebm.h"

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void Test_Alloc() ;     // Test behavior of Memory Model 
void Test_Thread() ;    // Test behavior of Threads
Return_t  prThreadProc(void* lpParam) ;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main() {
    
    // Test that it verifies changes in  New() and Delete() functions
    Test_Alloc();   
    
    Test_Thread();
    Test_Thread();

    return 0;
}
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


// max thread counter
enum {
 MAX_THREADS=5
} ;

// thread params(you can define yourself)
Struct( Thread_Param_t ) {
    ulong id;
    ulong nRandom;
    ulong nData;
} ;

// ------------------------------------------------------------

// thread process - print info about each thread
Return_t  prThreadProc(void* lpParam) {
    Thread_Param_t* pParam = (Thread_Param_t*)lpParam;
    // ...............[ Body of Function ].........................
    int nData = pParam->nData ;
    Sleep(200);
    printf("  ProcessId: %u,\tThread: %d,\tData: %d,\tRandom: %d\n"
                , Get_ProcessesId()
                , pParam->id
                , pParam->nData
                , pParam->nRandom
           );
    // ...............[ End Body of Function ].....................
    Delete(pParam) ; //free memory
    Return( nData ) ;
}

// ------------------------------------------------------------

void Test_Thread() {
    
    #ifdef EBM_THREAD
        char* msg1=(char*)"Using THREADS";
    #else
        char* msg1=(char*)"NO THREADS   ";
    #endif
    
    Thread_Param_t* pParam;
    Thread( objThreads, MAX_THREADS );
    
    Randomize();
    printf("\n"
           " ____________________________________________________________________________\n");
    printf("/.....................-=[ Begin Test: %s ]=-......................\\\n", msg1);
    for( int i=0 ; i<MAX_THREADS ; ++i ) {
        pParam = New( Thread_Param_t ) ;    // alloc heap

        if( pParam==NULL ) {
            printf("HeapAlloc error;\n");
            ExitProcess(2);
        }

        pParam->id       = i  ;
        pParam->nRandom = rand() ;
        pParam->nData   = (i+1) * 100 ;

        New_Threads(objThreads, i, prThreadProc, pParam);    // create thread with param

        if( objThreads.ahThread[i]==NULL )   ExitProcess(i);
    }

    Wait_Threads( objThreads );     // main thread waits for all objThreads finished
    printf(".........................................\n");
    
    for(int i=0 ; i < MAX_THREADS; ++i)
        printf( "\tThread: %d \tReturned: %d\n", i, Returned(objThreads, i) );
    printf("\\________________________________[ End Test ]________________________________/\n");
    /**/
    Close_Threads( objThreads );   // close handle of each thread
    
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void Test_Alloc() {
    long nRows = 3,
         nCols = 7;
    #ifdef EBM_THREAD
        printf("\n   IMPORTANT: EBM THREAD library by the moment is avaliable on Windows \n");
        printf(" ____________________________________________________________________________\n"
               "/............-=[ Begin Test: using Multi-Thread Model Memory ]=-.............\\\n");
    #else
        printf(" ____________________________________________________________________________\n"
               "/..............-=[ Begin Test: using Standard Model Memory ]=-...............\\\n");
    #endif
    
    long*   pDa    = New(long) ;
    long*   pArr   = New(long, nRows );
    long**  pMat1  = New(long, 4, 6 );
    long**  pMat   = New(long, nRows, nCols );
    
    printf(" 1. M[%d, %d]  \n", nRows, nCols );
    printf(" 2. M[%d, %d]  \n", nRows, nCols );
     
    Delete( pDa ) ;
    Delete( pArr, 0 ) ;
    Delete( pMat1, 4, 0 ) ;
    Delete( pMat, nRows, 0 ) ;

    Randomize(0);
    long**  *ppMat = New(long*, nRows, nCols );
    for(int i=0; i < nRows ; ++i )
        for(int j=0; j < nCols ; ++j ) {
            ppMat[i][j] = New(long) ; // Set Allocate Memory 
            *ppMat[i][j] = rand();
        }

    for(int i=0; i < nRows ; ++i ) {
        printf(" pMat[%d, 0..%d] = { ", i, nCols );
        for(int j=0; j < nCols-1 ; ++j )
            printf("%d, ", *(ppMat[i][j]) );
        printf("%d }\n", *(ppMat[i][nCols-1]) );
    }

    Delete_ptr( ppMat, nRows, nCols ) ;
    printf("\\________________________________[ End Test ]________________________________/\n");
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
