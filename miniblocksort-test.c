/***************************************************************************************
 * @Objetive : "MiniBlockSort test" Algorithm.
 * @version  : v1.0, Copyleft 2011.
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 ***************************************************************************************/
#define EBM_TEMPLATE
#define EBM_THREAD  // Comment for disable use of thread. Uncomment, use threads

#include "ebm/ebm.h"

#include "ebm/ebm-miniblocksort.h"
#include "ebm/ebm-miniblocksort1.h"
#include "ebm/ebm-miniblocksort2.h"
#include "ebm/ebm-miniblocksort3.h"
#include "ebm/ebm-miniblocksort4.h"

#include "ebm/ebm-miniblocksort-thread.h"   // This library is inable if 'USE_THREAD' is defined

#include "ebm/ebm-miniblocksort-c.h"
#include "ebm/ebm-miniblocksort-c2.h"



// ----------------------------------------------------------------------------

enum{
    MAX_SortMethods=30        // Maximun of Sort Methods to evaluate

    // ,CHECK_EVALUATIONS=1  
    // ,CHECK_EVALUATIONS=2  
    ,CHECK_EVALUATIONS=5  
    // ,CHECK_EVALUATIONS=10 
    // ,CHECK_EVALUATIONS=20 
    // ,CHECK_EVALUATIONS=25 
};


int gaSlots[] = {
              // 5, 6
              // , 7, 8, 9
          // ,  10, 25, 50, 75
          // , 100, 150, 185
          // , 190, 195                 // RADIXSORT
          // , 200, 215, 225, 235, 250
          // , 500
          // , 525, 530  // FLASHSORT
          // , 550
          // , 600, 650
          // , 700, 750
          // , 1000
          // , 5000
            10000
          , 50000
          , 100000
          , 250000
          , 261021 // maximun support by Tiny C.
          , 500000
          , 750000
          , 1000000
          // ,1500000
          // ,2061020 // maximun support by Tiny C.
          // ,2500000
          // ,3500000
          // ,5000000
          // ,10000000
          // ,100000000
    } ;

// ----------------------------------------------------------------------------

int main(int nArg, char** Args) {
    #ifdef EBM_THREAD
        printf("   *** USING THREAD ***\n");
    #else
        printf("   *** NO USING THREAD ***\n");
    #endif

    bool bShow_Array = false ;
    bool bIsAscending = ASC ;

    History_SortTest_t objHistory;

    Create_History_SortTest(  objHistory            // History Object
                            , gaSlots               // Array with list of Slots to test
                            , sizeofarray(gaSlots)  // slots in Array with list of Slots
                            , MAX_SortMethods       // Maximun quantities of sort methods to evaluate
                            , 0                     // Seed for Random generator
                            , 3000                  // Maximun value for Random generator
                            , CHECK_EVALUATIONS     // Evaluates the cycles
                            )  ;

    // Structure for Iterate over a History_SortTest_t object.
    //      <Type>  <iterator>  <type-for>,   <History_SortTest_t object>
    Foreach( int,   nIdSlots,   inhistory,   objHistory )
        
        printf("\n\n:::::::::::::::::::::::[ Test Array with %d slots ]:::::::::::::::::::::::\n", objHistory.nSlots ) ;
        QSort_Test(  nIdSlots, objHistory,  MiniBlockSort_Thread, bIsAscending,  bShow_Array,  LIST_RANDOM );
        
        QSort_Test(  nIdSlots, objHistory,  MiniBlockSort,        bIsAscending,  bShow_Array,  LIST_RANDOM );
        QSort_Test(  nIdSlots, objHistory,  MiniBlockSort4,       bIsAscending,  bShow_Array,  LIST_RANDOM );
        QSort_Test(  nIdSlots, objHistory,  MiniBlockSort1,       bIsAscending,  bShow_Array,  LIST_RANDOM );
        QSort_Test(  nIdSlots, objHistory,  MiniBlockSort3,       bIsAscending,  bShow_Array,  LIST_RANDOM );
        QSort_Test(  nIdSlots, objHistory,  MiniBlockSort2,       bIsAscending,  bShow_Array,  LIST_RANDOM );
        
        // QSort_Test(  nIdSlots, objHistory,  MiniBlockSort_c,      bIsAscending,  bShow_Array,  LIST_RANDOM );
        // QSort_Test(  nIdSlots, objHistory,  MiniBlockSort_c2,     bIsAscending,  bShow_Array,  LIST_RANDOM );
        
        
    End(inhistory, objHistory)
    printf("---------------------------------------------------------------\n");
    // Generate_Results( objHistory, LIST_RANDOM ) ; // Show result in: console, console error and data file.

   Close_History_SortTest( objHistory ) ;


   return 0;
}
