/***************************************************************************************
 * @Objetive  : Test the "Quicksort" Algorithm.
 * @version   : v1.0, Copyleft 2011-Set/2017. Arequipa, Peru.
 * @Author    : Edgard Medina Q. <edgardmedina@yahoo.com>
 *                               <edgard.medina@ucsp.edu.pe>
 *
 * Obervations: I use many private control structures and macro functions to
 *              simplify the code, they are implemented in "ebm/ebm.h"
 *              this code exploit feature of GNU C99.
 ***************************************************************************************
 *
 * History_SortTest_t     Type Object to define a history about sort method test
 *
 *
 * void  Create_History_SortTest(
 *             History_SortTest_t ByRef p_pObjHistory          // Name of New Object of history of evaluations.
 *           , size_t                   p_nItem_SlotsInArray   // How many sizes of Items for array are tested.
 *           , size_t                   p_nMaxSortMethodsEval  // Maximun quantity of sort methos to tested.
 *           , size_t                   p_nRandSeed            // Seed for Randonmize function.
 *           , long                     p_nMaxValue            // Top máximun values for random generator.
 *          ) ;
 *
 * void  _Delete_History_SortTest(
 *                  History_SortTest_t ByRef  p_objHistory       // Name of History_SortTest Object to delete.
 *               ) ;
 *
 ***************************************************************************************/

#define EBM_TEMPLATE
#define EBM_THREAD  // Comment for disable use of threads. Uncomment, use threads
#include "ebm/ebm.h"


#include "ebm/ebm-miniblocksort.h"
#include "ebm/ebm-miniblocksort-thread.h"   // Miniblocksort using Threads.
// #include "ebm/ebm-miniblocksort-c.h"        // Miniblocksort in pure C with pointers
// #include "ebm/ebm-miniblocksort-c2.h"       // Miniblocksort in pure C with pointers


#include "sorts/radixsortlsd.h"
#include "sorts/flashsort.h"
#include "sorts/insertsort.h"
#include "sorts/mergesort.h"
// #include "sorts/mergesort2.h"
#include "sorts/mergesort_inplace.h"
#include "sorts/quicksort.h"
// #include "sorts/gnu-quicksort2.h"
#include "sorts/qsort3way-c.h"
// #include "sorts/quicksort3way-bently.h"
#include "sorts/dualpivot-quicksort.h"
#include "sorts/shellsort.h"
// #include "sorts/binary_insertsort.h"
#include "sorts/timsort.h"
// // #include "sorts/qsort_bently.h"

//--------------------------------------------------------------------------------------------

enum {
   // CHECK_EVALUATIONS = 2 
   // CHECK_EVALUATIONS = 5  
   CHECK_EVALUATIONS = 25 

   ,MAX_SORT_METHODS=60  // Quantity of Sort Methods to evaluate.
};


// Array with a list of slots (size of array) for the arrangement of tests.
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

        // 700
      // , 750
      // , 1000
      // , 5000
      // , 10000
      // , 50000
      // , 100000

        100000
      , 250000
      , 400000
      , 550000
      , 700000
      , 850000
      , 1000000
} ;

static int gnItem_SlotsInArray = sizeofarray(gaSlots);  // sizeof(gaSlots)/sizeof(*gaSlots)


//--------------------------------------------------------------------------------------------

int main(int nArg, char** Args) {
    bool bIsAscending = true ;    // true : sort is ascending; false: sort is descending (dónt work for all sort)
    bool bShowArray   = false ;   // true: show the array; false: don't show the array.
        
    const int nRandSeed  = 0;     // Seed for Randonmize function.
    const int gnMaxValue = 3000 ; // Top máximun values for random generator

    History_SortTest_t objHistory;

    Create_History_SortTest(  objHistory            // History Object
                            , gaSlots               // Array with list of Slots to test
                            , sizeofarray(gaSlots)  // slots in Array with list of Slots
                            , MAX_SORT_METHODS      // Maximun quantities of sort methods to evaluate
                            , nRandSeed             // Seed for Random generator
                            , gnMaxValue            // Maximun value for Random generator
                            , CHECK_EVALUATIONS     // Evaluates the cycles
                            )  ;

    // Structure for Iterate over a custom list
    //       <Type>      <iterator>  <type-for>,   < list of values ... >
    Foreach( Type_List, idPattern,   inlist
                , LIST_SORTED,    LIST_REVERSED,  LIST_RANDOM
                // , LIST_FEWUNIQUE, LIST_SAWTOOTH,  LIST_STAGGER
                // , LIST_PLATEAU,   LIST_SHUFFLE
            ) 


        // Structure for Iterate over a History_SortTest_t object.
        //      <Type>  <iterator>  <type-for>,   <History_SortTest_t object>
        Foreach( int,   nIdSlots,   inhistory,   objHistory ) 
            // ...............................
            Sort_Test( long, nIdSlots, objHistory, RadixSortLSD,       bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, FlashSort,          bIsAscending, bShowArray, idPattern );

            Sort_Test( long, nIdSlots, objHistory, DualPivotQuickSort, bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, qsort3way,          bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, QuickSort3way,      bIsAscending, bShowArray, idPattern ); // Revizar: falla con lista ordenadas Ascendente

            Sort_Test( long, nIdSlots, objHistory, QSort,              bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, QuickSort,          bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, gnuQuicksort,       bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, MergeSort,          bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, MergeSort2,         bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, Mergesort_InPlace,  bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, TimSort,            bIsAscending, bShowArray, idPattern );

            Sort_Test( long, nIdSlots, objHistory, MiniBlockSort,      bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, MiniBlockSort_Thread,  bIsAscending, bShowArray, idPattern );
            
            // Sort_Test( long, nIdSlots, objHistory, MiniBlockSort_c,    bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, MiniBlockSort_c2,   bIsAscending, bShowArray, idPattern );
            
            
            
            // Sort_Test( long, nIdSlots, objHistory, ShellSort,          bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, InsertSort,         bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, BinaryInsertSort,   bIsAscending, bShowArray, idPattern );
            // ...............................
        End(inhistory, objHistory)
        
        Generate_Results( objHistory, idPattern ) ; // Show result in: console, console error and data file.
        
    End(inlist)

    Close_History_SortTest( objHistory ) ;
    return 0;
}

// ...................[ End programm ]................
