/***************************************************************************************
 * @Objetive : Test the "Mergesort" Algorithm.
 * @version  : v1.0, Copyleft 2011.
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 ***************************************************************************************/
 
#define EBM_TEMPLATE
#include "ebm/ebm-sort-test.h"

#include "sorts/shellsort.h"

enum {
   // CHECK_EVALUATIONS = 1 + 1
   // CHECK_EVALUATIONS = 2 + 1
   // CHECK_EVALUATIONS = 5 + 1
   CHECK_EVALUATIONS = 25 + 1

   ,MAX_SORT_METHODS=60  // Quantity of Sort Methods to evaluate.
};


// Array with a list of slots (size of array) for the arrangement of tests.
int gaSlots[] = {
      // 5, 6, 7, 8, 9, 10, 25
       50, 75
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
      // ,
        // 100000
      // , 250000
      , 261021    // maximun support by Tiny C.
      // , 400000
      , 520964    // maximun support by Tiny C (ok).
      // , 521921    // maximun support by Tiny C.
      // , 550000
      // , 700000
      // , 721921    // maximun support by Tiny C.
      // , 850000
      // , 921921    // maximun support by Tiny C.
      // , 1000000
      // , 1500000   // maximun support by Tiny C.
} ;

static int gnItem_SlotsInArray = sizeofarray(gaSlots);  // sizeof(gaSlots)/sizeof(*gaSlots)

int main(int nArg, char** Args) {
    bool bIsAscending = true ;    // true : sort is ascending; false: sort is descending (dónt work for all sort)
    bool bShowArray   = false ;   // true: show the array; false: don't show the array.
        
    const int nRandSeed  = 0;     // Seed for Randonmize function.
    const int gnMaxValue = 3000 ; // Top máximun values for random generator

    History_SortTest_t objHistory;

    Create_History_SortTest(  objHistory            // History Object
                            , gaSlots               // Array with list of Slots to test
                            , gnItem_SlotsInArray   // slots in Array with list of Slots
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
            Sort_Test( long, nIdSlots, objHistory, ShellSort,  bIsAscending, bShowArray, idPattern );
            Sort_Test( long, nIdSlots, objHistory, ShellSort,  bIsAscending, bShowArray, idPattern );
            // Sort_Test( long, nIdSlots, objHistory, SellSort2,  bIsAscending, bShowArray, idPattern );
        End(inhistory, objHistory)
        
        Generate_Results( objHistory, idPattern ) ; // Show result in: console, console error and data file.
        
    End(inlist)

    Delete_History_SortTest( objHistory ) ;
    return 0;
}

// ...................[ End programm ]................
