/***************************************************************************************
 * @Objetive : Test the "Mergesort" Algorithm.
 * @version  : v1.0, Copyleft 2011.
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 ***************************************************************************************/
// #define EBM_TEMPLATE

#include "ebm/ebm.h"

#include "sorts/quicksort.h"
#include "sorts/quicksort_c.h"

// ----------------------------------------------------------------------------

enum{
      _ID_MiniBlockSort=0
    
    , MAX_SortMethods=30        // Maximun of Sort Methods to evaluate
    
    // ,CHECK_EVALUATIONS=1  + 1
    ,CHECK_EVALUATIONS=2  + 1
    // ,CHECK_EVALUATIONS=5  + 1
    // ,CHECK_EVALUATIONS=10 + 1
};

#define Test( _IgnoreStat, _SortMethod, _pArray, _nSlots, _bIsAscending,  _bShow_Array, _aRecord, _IdxRec ) \
    Procedure\
        if( _IdxRec < MAX_SortMethods ) {\
            _aRecord[_IdxRec] = New(RecordTime_t);\
            strcpy( _aRecord[_IdxRec]->szMethod, STR(_SortMethod) );\
            size_t nStartTime, nEndTime;\
            if( _IgnoreStat ) printf("\n  ---------------[ %s Ascending / Evaluations: %d ----------------\n", STR(_SortMethod), CHECK_EVALUATIONS-_IgnoreStat );/**/\
            for(int nIterEval=0; nIterEval < CHECK_EVALUATIONS ; ++nIterEval ) {\
                Randomize(0);\
                for( int i=0; i< _nSlots ; ++i ) \
                  _pArray[i] = Rand(30000);\
                \
                nStartTime = gettime_nsec() ;\
                _SortMethod( _pArray, _nSlots, long, _bIsAscending ) ; \
                _aRecord[_IdxRec]->anTime[nIterEval] = Elapsed(nStartTime, gettime_nsec() ) ;\
            }\
            if( _IgnoreStat == IGNORE ) \
                Delete( _aRecord[_IdxRec]) ;\
            else {\
                Statistics_From_RecordTime( _aRecord[_IdxRec], CHECK_EVALUATIONS, false );\
                if( _bShow_Array )  show_array(long, "%d", _pArray, _nSlots); /**/\
                Check_Sorted(long, _pArray, _nSlots, _bIsAscending) ;/**/\
                printf("    Time | avg: %f ms   min: %f ms   max: %f ms\t\n" \
                                        , nsec2msec( _aRecord[_IdxRec]->nAvgTime) \
                                        , nsec2msec( _aRecord[_IdxRec]->nMinTime) \
                                        , nsec2msec( _aRecord[_IdxRec]->nMaxTime) \
                                                     );\
                ++_IdxRec;\
            }\
        } else\
                printf("   The evaluation for '%s' cann't be made, beacuse execed maximun allowed.\n", STR(_fnComp));\
    EndProcedure

// ----------------------------------------------------------------------------

int main(int nArg, char** Args) {


   // size_t nSlots = 1041738 ; // maximun support by Tiny C (Ok).
   // size_t nSlots = 1041750 ;// maximun support by Tiny C (ok).
   // size_t nSlots = 1000000 ;// maximun support by Tiny C (ok).
   // size_t nSlots = 520964 ;// maximun support by Tiny C (ok).
   size_t nSlots = 261021 ;
   // size_t nSlots = 1000 ;
   // size_t nSlots = 100 ;
   // size_t nSlots = 26 ;
   // size_t nSlots = 27 ;

    int gaSlots[] = {
              5, 6
              , 7, 8, 9
          ,  10, 25, 50, 75
          , 100, 150, 185
          , 190, 195                 // RADIXSORT
          , 200, 215, 225, 235, 250
          , 500
          , 525, 530  // FLASHSORT
          , 550
          , 600, 650
          , 700, 750
          // , 1000
          // , 5000
          // , 10000
          // , 100000
          // ,250000
          // ,261021 // maximun support by Tiny C.
          // ,500000
          // ,750000
          // ,1000000
          // ,1500000
          // ,2061021 // maximun support by Tiny C.
          // ,2500000
          // ,3500000
          // ,5000000
          // ,10000000
          // ,100000000
    } ;
    size_t gnSlots = sizeofarray(gaSlots);

   // .........................................
   
    bool bShow_Array = false ;
    // bool bShow_Array = true ;

    long *pArray ;
    bool isAscending = ASC ;

    int nRecUsed;
    RecordTime_t**  aRecord_Cmp = New( RecordTime_t*, MAX_SortMethods ) ;
    nRecUsed = 0 ;

    // for(int k=0; k< gnSlots; ++k) {
        // nSlots = gaSlots[k];

        pArray = New( long, nSlots ) ;
		
        Test( IGNORE,    QuickSort,  pArray, nSlots, isAscending, bShow_Array, aRecord_Cmp, nRecUsed );
        
        Test( NOIGNORE,  QuickSort,   pArray, nSlots, isAscending, bShow_Array, aRecord_Cmp, nRecUsed );
        Test( NOIGNORE,  QuickSort_c, pArray, nSlots, isAscending, bShow_Array, aRecord_Cmp, nRecUsed );
		
        Delete( pArray, 0 ) ;
/**/
    // }
   printf("\n      .................................................................\n", nSlots);

   return 0;
}
