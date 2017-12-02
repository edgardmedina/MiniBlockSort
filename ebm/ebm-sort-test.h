#ifndef __ebm_sorttest_h__
#define __ebm_sorttest_h__

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * This structure maintains a record of the tests performed for each sort method
 * in each array with certain size.
 */
Struct(History_SortTest_t) {
    RecordTime_t** *pTable_Record,  /* Table to store time results of each sort method to will be
                                     tested over an array with N Slots (cells/elements). */
                ** *pRanking_Aux; /* Auxiliar Table to store the Ranking of sort method sorted
                                     by time, tested in 'pTable_Time'.*/
    size_t      nRows,            /* Rows   in 'pTable_Time' and 'pRanking_Aux': How many sizes of array are tested. */
                nCols,            /* Colums in 'pTable_Time' and 'pRanking_Aux': Maximun quantity of sort methods to tested. */
                nCurrItem ;       // Auxiiliar field for use in iterate loops.
                
    int         nCountMethod ;    // Counter of how many sort methods were evaluated.

    long *    pArray;             // Pointer to Dinamic Array for testing.
    size_t    nSlots;             // Slots in Dinamic Array.

    Type_List nPattern;           // Pattern List Identifier to generate in array.
    char*     szPattern;          // Pattern List name.

    size_t    nSeed ;             // Seed for Random Generator.
    short     nMaxValue;          // Max Value for random generator.

    long     *pTestSlots;         // Array with list of quantity of Slots to test
    size_t    nTestSlots;         // Quantity of Slots in Array List of Slots.

    size_t    nEvaluations;       // Quantity for check evaluations.
};

//--------------------------------------------------------------------
#define Foreach_inhistory( _Type, _Iter, _ObjHistory ) \
        for(size_t _Iter = 0 ; _Iter < _ObjHistory.nTestSlots ; ++_Iter ) { \
            _ObjHistory.nCurrItem = _Iter ; \
            _ObjHistory.nCountMethod = 0 ;                         /* Counts the number of evaluated methods*/ \
            _ObjHistory.nSlots = _ObjHistory.pTestSlots[_Iter] ;   /* Get new slots quantity for testing array.*/ \
            _ObjHistory.pArray = New( long, (_ObjHistory.nSlots) ) ; /* Allocate memory for a new size of array.*/ \

#define End_inhistory( _ObjHistory ) \
            Rankig_Record_Array(  _ObjHistory.pRanking_Aux[_ObjHistory.nCurrItem] \
                                , _ObjHistory.nCountMethod \
                                , Compare_RecordTime_AvgTime ); \
            \
            Delete( _ObjHistory.pArray ) ;  /* free memory from array.*/ \
        }

//--------------------------------------------------------------------

#define  Create_History_SortTest(  _ObjHistory \
                                 , _pTestSlots \
                                 , _nItem_SlotsInArray \
                                 , _nMaxSortMethodsEval \
                                 , _nRandSeed \
                                 , _nMaxValue \
                                 , _nEvaluations \
                                )  \
        _Create_History_SortTest(  &_ObjHistory \
                                 , (_pTestSlots) \
                                 , (_nItem_SlotsInArray) \
                                 , (_nMaxSortMethodsEval) \
                                 , (_nRandSeed) \
                                 , (_nMaxValue) \
                                 , (_nEvaluations) \
                                )

void  _Create_History_SortTest(
                  History_SortTest_t *p_pObjHistory          // Name of New Object of history of evaluations.
                , long               *p_pTestSlots           // Array with List of Slots to test.
                , size_t              p_nItem_SlotsInArray   // How many sizes of Items for array are tested.
                , size_t              p_nMaxSortMethodsEval  // Maximun quantity of sort methos to tested.
                , size_t              p_nRandSeed            // Seed for Randonmize function.
                , long                p_nMaxValue            // Top máximun values for random generator.
                , size_t              p_nEvaluations         // Quantity for check evaluations.
               ) {

    p_pObjHistory->pTestSlots = p_pTestSlots ;             // Array with list of quantity of Slots to test
    p_pObjHistory->nTestSlots = p_nItem_SlotsInArray ;              // Quantity of Slots in Array List of Slots.

    p_pObjHistory->nRows = p_nItem_SlotsInArray  ;         // Set Maximun quantity os sizeses to evaluate.
    p_pObjHistory->nCols = p_nMaxSortMethodsEval ;         // Set Maximun qunatity of sort methods to evaluate.

    p_pObjHistory->nCountMethod = 0 ;                      // Set to zero, counter for sort methods to will be evaluated.

    p_pObjHistory->pTable_Record = New( RecordTime_t*,       // Create a Table that pointer to Structure of type 'RecordTime_t'.
                                       (p_pObjHistory->nRows),  // Rows: how many sizes os array are tested.
                                       (p_pObjHistory->nCols)   // Cols: Maximun quantity of sort methos to tested.
                                      ) ; 
    p_pObjHistory->pRanking_Aux = New( RecordTime_t*,      // Create a Auxuiliar Table to store the Ranking of sort methods
                                       (p_pObjHistory->nRows),  // Rows: how many sizes os array are tested.
                                       (p_pObjHistory->nCols)   // Cols: Maximun quantity of sort methos to tested.
                                     ) ; 

    p_pObjHistory->nSeed     = p_nRandSeed;                // Set the SEED for Randomize function.
    p_pObjHistory->nMaxValue = p_nMaxValue;                // Set Max Value for Random to generate,

    p_pObjHistory->nEvaluations = p_nEvaluations + 1 ;
}
/* ....................  */

#define  Close_History_SortTest( _objHistory_SortTest )  _Close_History_SortTest( &_objHistory_SortTest )
void    _Close_History_SortTest(
                  History_SortTest_t*  p_pObjHistory           // Name of History_SortTest Object to delete.
               ) {
    Delete_ptr( p_pObjHistory->pTable_Record, (p_pObjHistory->nRows), (p_pObjHistory->nCols) ) ;  // Free memory from table 'pTable_Record'. NOTE: Not all cols for methods are used, only are reserved
    Delete(     p_pObjHistory->pRanking_Aux,  (p_pObjHistory->nRows), 0);                         // Free memory from table 'pRanking_Aux'

    p_pObjHistory->nRows = 0 ;         // Set Maximun quantity os sizeses to evaluate.
    p_pObjHistory->nCols = 0 ;         // Set Maximun qunatity of sort methods to evaluate.
    p_pObjHistory->nCountMethod = 0 ;                      // Set to zero, counter for sort methods to will be evaluated.

    p_pObjHistory->nSeed     = 0;          // Set the SEED for Randomize function.
    p_pObjHistory->nMaxValue = INT16_MAX ; // Set Max Value for Random to generate,

    p_pObjHistory->pTestSlots = NULL ;     // Release Array with list of quantity of Slots to test
    p_pObjHistory->nTestSlots = 0    ;     // Set to zero .

}

// -----------------------------------------------------------------------------


static void _Show_Partial_Results( FILE* pStdOut, History_SortTest_t *p_pHistory_SortTest ) {
    fprintf(pStdOut, "\n");
    for( size_t nIdSlot = 0 ; nIdSlot < p_pHistory_SortTest->nRows ; ++nIdSlot ) {
        fprintf(pStdOut, "\n***********************[ Id: %d  / Slots: %d  / Evaluations: %d ]*****************************\n", nIdSlot
                                                                                                                   , p_pHistory_SortTest->pTestSlots[nIdSlot]
                                                                                                                   , p_pHistory_SortTest->nEvaluations-1 );
        fprintf(pStdOut, "%18s Time (ms):  \t\t%s\t\t| %s\t%s\t  | List od Times [%d evaluation(s)]"
                // " %s %s %s\n"
                "\n"
                    , "Method", "AvgTime", "MinTime", "MaxTime"
                    , p_pHistory_SortTest->nEvaluations-1
                    , "Method", "msec", "MinTime", "AvgTime", "MaxTime"
                    , "Max-Avg", "Avg-Min", "Max_Avg-Avg_Min" );

        llong nDiff_Avg_Min,
              nDiff_Max_Avg;
        RecordTime_t *pRegTime ;

        for( int idReg=0; idReg < p_pHistory_SortTest->nCountMethod ; ++idReg ) {
            pRegTime = p_pHistory_SortTest->pRanking_Aux[nIdSlot][idReg] ;

            nDiff_Avg_Min = pRegTime->nAvgTime - pRegTime->nMinTime ,
            nDiff_Max_Avg = pRegTime->nMaxTime - pRegTime->nAvgTime ;

            fprintf(pStdOut, "%19s [%s-%s] (%d):\t"
                   "%4.6f\t| %4.6f\t%4.6f  |"
                   // "\t %3.6f |"
                   // "(%3lld)  (%3lld) | (%3lld)"
                   // ""
                   ,  pRegTime->szMethod
                   , (pRegTime->array_TypeSorted ? "ASC" : "DESC")
                   , (pRegTime->array_IsSorted ? "Ok" : "Fail")
                   , idReg

                   , nsec2msec(pRegTime->nAvgTime)
                   , nsec2msec(pRegTime->nMinTime)
                   , nsec2msec(pRegTime->nMaxTime)

                   , nsec2msec(pRegTime->nSelTime)

                   , nDiff_Max_Avg
                   , nDiff_Avg_Min
                   , nDiff_Max_Avg - nDiff_Avg_Min
                 ) ;
              for(int nEval=1; nEval < p_pHistory_SortTest->nEvaluations; ++nEval )
                 fprintf(pStdOut, "\t%4.6f", nsec2msec(pRegTime->anTime[nEval]) ) ;
              fprintf(pStdOut, "\n");
        }
    }
    // fprintf(pStdOut, "\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
}


static void _Show_Table_Results( FILE* pStdOut, History_SortTest_t* p_pObjHistory,  Type_List p_nPandomPattern, bool p_bShowRanking ) {
    // fprintf(pStdOut, "    --> avg: %f\n", nsec2msec((pTable_Time[0][1])->nAvgTime) ) ;
    int idSlots = 0;
    fprintf(pStdOut, "\n\n\t\t\t\tPatron: %s  (El tiempo promedio se obtuvo de %d evaluaciones)\n", p_pObjHistory->szPattern, p_pObjHistory->nEvaluations-1 );
    fprintf(pStdOut, "-----------------------------------------------------------------------------------------------------------------------\n");
    fprintf(pStdOut, "%16s   ", "Method");
    for(int nSlots = 0 ; nSlots < p_pObjHistory->nTestSlots ; ++nSlots )
        fprintf(pStdOut, "\t%7d", p_pObjHistory->pTestSlots[nSlots] ) ;
    fprintf(pStdOut, "\n");
    fprintf(pStdOut, "-----------------------------------------------------------------------------------------------------------------------\n");
    RecordTime_t *pRegTime ;


    for( int nMethod= 0; nMethod < p_pObjHistory->nCountMethod ; ++nMethod ) {
        fprintf(pStdOut, "%19s: ",  p_pObjHistory->pTable_Record[0][nMethod]->szMethod );

        for(int nSlots = 0 ; nSlots < p_pObjHistory->nTestSlots ; ++nSlots ) {
            pRegTime = p_pObjHistory->pTable_Record[nSlots][nMethod] ;
            if( p_bShowRanking )
                fprintf(pStdOut, "\t%4.6f (%d)", nsec2msec(pRegTime->nAvgTime)
                                     , pRegTime->nRanking ) ;
            else
                fprintf(pStdOut, "\t%4.6f", nsec2msec(pRegTime->nAvgTime) ) ;
       }

        fprintf(pStdOut, "\n" );
   }
    fprintf(pStdOut, "-----------------------------------------------------------------------------------------------------------------------\n");
}

//--------------------------------------------------------------------------------------------
void _Generate_Ouput( History_SortTest_t * p_pObjHistory, Type_List p_nIdPattern, bool p_bAdjustTime ) {
    // Get ifnormation for Slots Evaluation.
    char szSlots[20];
    char szFile[30];
    RecordTime_t* pRecord;
    int nStarIn = p_bAdjustTime 
                    ? (p_pObjHistory->nEvaluations<2 ? 0 : 1) 
                    : 0 ;
    
    for(int nIdSlots = 0 ; nIdSlots < p_pObjHistory->nRows ; ++nIdSlots ) {
        itoa( p_pObjHistory->pTestSlots[nIdSlots], szSlots, 10 );
        sprintf(szFile, "%d-eval_slots-%07d_%s.dat"
                      , p_pObjHistory->nEvaluations
                      , p_pObjHistory->pTestSlots[nIdSlots]
                      , gszRandomPattern[(int)p_nIdPattern]
              ) ;
        
        FILE* pfile = fopen( szFile, "w");
        fprintf(pfile, "# IdMethod\tMethod\tRanking\tTime(ms)\n");
        for(int nMethod=0; nMethod < p_pObjHistory->nCountMethod ; ++nMethod ) {
            
            pRecord = p_pObjHistory->pRanking_Aux[nIdSlots][nMethod] ;
            
            for(int nEval = nStarIn ; nEval < p_pObjHistory->nEvaluations ;  ++nEval ) 
                fprintf(pfile, "%d\t%s\t%d\t%4.6lf\n"
                             , nMethod              // Id Sort Method
                             , pRecord->szMethod    // Name Sort Method
                             , pRecord->nRanking    // Ranking Sort Method
                             , nsec2msec(pRecord->anTime[nEval])        // Time in Evaluation
                       ) ;
        }
        fclose( pfile );
    }
}

#define  Generate_Results( _objHistory, _nType_Pattern ) \
            _Show_History_Results( &_objHistory, (Type_List)_nType_Pattern )

void _Show_History_Results( History_SortTest_t *p_pHistory, Type_List  p_nType_Pattern ) {

    _Show_Table_Results( stdout, p_pHistory, p_nType_Pattern, false );
    _Show_Table_Results( stdout, p_pHistory, p_nType_Pattern, true  );
    
    printf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    
    _Show_Table_Results( stderr, p_pHistory, p_nType_Pattern, true );
    _Show_Partial_Results( stderr, p_pHistory ) ;

    fprintf(stderr, "\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");

    _Generate_Ouput( p_pHistory, p_nType_Pattern, true) ;
    
}

//--------------------------------------------------------------------
//Macro Function to test a sort method

#define Sort_Test(  _Type             /* ANSI C Types */ \
                  , _nIdSlotsInArray  /* Identifier for Slots in Array. */ \
                  , _objHistory       /* History Object */ \
                  , _Sort_Method      /* Method to sort: QuickSort, MergeSort, etc.; it must has the syntax:
                                                void Sort( Type* array, size_t SizeArray, bool isAscending ) */ \
                  , _bAscending       /* Flag to sort: ASC or DESC */ \
                  , _bShowArray       /* Flag to show th elements of array. */ \
                  , _nType_Pattern    /* Type of Pattern list: LIST_RANDOM, LIST_SORTED, LIST_REVERSED, LIST_FEWUNIQUE  */ \
                 )\
    Procedure\
        objHistory.nPattern = _nType_Pattern ; \
        objHistory.szPattern = gszRandomPattern[_nType_Pattern] ;\
        \
        llong  nStartProcessTime \
             , nElapsedProcessTime; \
        llong   nElapsed \
              , nStartTime; \
        llong   nMinTime = 100000000 \
              , nMaxTime = 0; \
        \
        RecordTime_t*  pRecordTime = New( RecordTime_t ) ;\
        \
        _objHistory.pTable_Record[_nIdSlotsInArray][_objHistory.nCountMethod] = pRecordTime ; \
         _objHistory.pRanking_Aux[_nIdSlotsInArray][_objHistory.nCountMethod] = pRecordTime ; \
        \
        pRecordTime->array_TypeSorted= _bAscending ; \
        pRecordTime->array_slots = _objHistory.nSlots ; \
        pRecordTime->nRanking    = 0 ; \
        pRecordTime->nSelTime    = 0 ; \
        pRecordTime->nMinTime    = 0 ; \
        pRecordTime->nMaxTime    = 0 ; \
        pRecordTime->nAvgTime    = 0 ; \
        strcpy(pRecordTime->szMethod, STR(_Sort_Method) ) ;\
        \
        if( not( /*   ( strcmp(STR(_Sort_Method), "MergeSort"       ) == 0 and _objHistory.nSlots > gnMergeTop ) \
                 /*or ( strcmp(STR(_Sort_Method), "QuickSort"       ) == 0 and _objHistory.nSlots > 1000000-1 ) /**/\
                 ( strcmp(STR(_Sort_Method), "ShellSort"       ) == 0 and _objHistory.nSlots >= 100000 ) or /**/ \
                 ( strcmp(STR(_Sort_Method), "InsertSort"      ) == 0 and _objHistory.nSlots > 60000 ) or\
                 ( strcmp(STR(_Sort_Method), "BinaryInsertSort") == 0 and _objHistory.nSlots > 60000 ) ) \
          ) { \
            nStartProcessTime = gettime_nsec();\
            int nControl ; \
            for(int nEval=0; nEval < _objHistory.nEvaluations; ++nEval ) { \
                nControl = (nEval==0 ? 2 : 1 ) ; \
                do { \
                    Generate_Pattern_List(  _objHistory.pArray \
                                          , _objHistory.nSlots \
                                          , _nType_Pattern \
                                          , _objHistory.nMaxValue \
                                          , _objHistory.nSeed  \
                                         ); /* Generate a new pattern list in array. */ \
                    \
                    if( _bShowArray ) \
                        show_array(long, "%d", _objHistory.pArray, _objHistory.nSlots ); \
                    \
                    nStartTime = gettime_nsec(); \
                    _Sort_Method( _objHistory.pArray, \
                                  _objHistory.nSlots, \
                                  _Type, \
                                  _bAscending ) ; \
                    nElapsed = Elapsed( nStartTime, gettime_nsec() ) ; \
                    /* --[Control de Prioridades del Programador de Tareas del Sist. Operativo]--*/ \
                    /*if( nElapsed < nMinTime ) nMinTime = nElapsed ; \
                    if( nEval < 2 ) { \
                        if( nMaxTime > 0 ) { \
                             if( nElapsed < nMaxTime ) nMaxTime = nElapsed ; \
                        } else \
                            nMaxTime = nElapsed ; \
                    } \
                    if( nControl > 0 ) --nControl; \
                    if( nControl==0) { \
                        if( nElapsed <= nMaxTime ) break ; \
                        if( nElapsed >  nMaxTime and 8*nMaxTime > nElapsed ) break ; \
                    } /**/\
                    /* ----------------------------------------------*/ \
                } while( false ) ;\
                if( nElapsed > nMaxTime  ) nMaxTime = nElapsed ; \
                \
                pRecordTime->anTime[nEval] = nElapsed ; \
                \
                if( _bShowArray ) \
                    show_array(long, "%d", _objHistory.pArray, _objHistory.nSlots );\
            } \
            nElapsedProcessTime = Elapsed( nStartProcessTime, gettime_nsec() ) ;\
            pRecordTime->array_IsSorted = IsArraySorted( long, _objHistory.pArray, _objHistory.nSlots, _bAscending ) ; \
            Statistics_From_RecordTime( pRecordTime, _objHistory.nEvaluations, true ) ; \
        } \
        _objHistory.nCountMethod++ ; \
   EndProcedure

//--------------------------------------------------------------------

#define QSort_Test( _IdxRec, _objHistory, _SortMethod, _bIsAscending, _bShow_Array, _nType_Pattern ) \
    Procedure\
        RecordTime_t * pRecTime ; \
        if( _objHistory.nCurrItem < _objHistory.nCols ) { \
            size_t nStartTime, \
                   nEndTime; \
            int nTimes = (_objHistory.nCurrItem == 0 ? 2 : 1) ; /* This line is to get attention from the Processor's Schedule */\
            /*printf(" _objHistory.nCurrItem = %d\tTimes=%d\n", _objHistory.nCurrItem, nTimes) ;/**/\
            while( nTimes-- ) { \
                pRecTime = New( RecordTime_t ) ;\
                \
                _objHistory.pTable_Record[_IdxRec][_objHistory.nCountMethod] = pRecTime ; \
                 _objHistory.pRanking_Aux[_IdxRec][_objHistory.nCountMethod] = pRecTime ; \
                \
                pRecTime->array_TypeSorted= _bIsAscending ; \
                pRecTime->array_slots = _objHistory.nSlots ; \
                pRecTime->nRanking    = 0 ; \
                pRecTime->nSelTime    = 0 ; \
                pRecTime->nMinTime    = 0 ; \
                pRecTime->nMaxTime    = 0 ; \
                pRecTime->nAvgTime    = 0 ; \
                strcpy(pRecTime->szMethod, STR(_SortMethod) ) ;\
                \
                if( not(_IdxRec == 0 and nTimes == 1) ) \
                    printf("\n  -----------[ %s Ascending / Evaluations: %d ]------------\n" \
                            , pRecTime->szMethod \
                            , _objHistory.nEvaluations-1 ); \
                for(int nEval=0; nEval < _objHistory.nEvaluations ; ++nEval ) {\
                    Generate_Pattern_List(  _objHistory.pArray \
                          , _objHistory.nSlots \
                          , _nType_Pattern \
                          , _objHistory.nMaxValue \
                          , _objHistory.nSeed  \
                         );     /* Generate a new pattern list in array. */ \
                    \
                    nStartTime = gettime_nsec() ; \
                    _SortMethod( _objHistory.pArray, _objHistory.nSlots, long, _bIsAscending ) ; \
                    nEndTime = gettime_nsec() ; \
                    pRecTime->anTime[nEval] = Elapsed( nStartTime, nEndTime ) ; \
                } \
                if( _IdxRec == 0 and nTimes == 1 ) \
                    Delete( pRecTime ) ;\
            } \
            pRecTime->array_IsSorted = IsArraySorted( long, _objHistory.pArray, _objHistory.nSlots, _bIsAscending ) ; \
            Statistics_From_RecordTime( pRecTime, _objHistory.nEvaluations, true );\
            if( _bShow_Array )  \
                show_array(long, "%d", _objHistory.pArray, _objHistory.nSlots ); \
            Check_Sorted(long, _objHistory.pArray, _objHistory.nSlots, _bIsAscending) ;\
            printf("    Sorted in avg: %f ms   min: %f ms   max: %f ms\t\n" \
                                    , nsec2msec( pRecTime->nAvgTime) \
                                    , nsec2msec( pRecTime->nMinTime) \
                                    , nsec2msec( pRecTime->nMaxTime) \
                                    , nsec2msec( pRecTime->nSelTime) \
                                                 );\
            _objHistory.nCountMethod++ ; \
        } else\
                printf("   The evaluation for '%s' cann't be made, beacuse execed maximun allowed.\n", STR(_fnComp));\
    EndProcedure

//--------------------------------------------------------------------
          
#ifdef __cplusplus
    }
#endif

#endif      // #ifndef __ebm_sorttest_h__