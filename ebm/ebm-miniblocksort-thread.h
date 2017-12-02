/***************************************************************************************
 * @Algorithm : MiniBlock Sort.
 * @Objetive  : Sort algorithm for generic data, is stable.
 * @version   : v1.0, Copyleft 2012 - Sep/2017
 * @modified  : October/2017
 * @Author    : Edgard Medina Q. <edgardmedina@yahoo.com>
 *              Arequipa, Perú
 ***************************************************************************************
 * Syntax:
 *  void MiniBlockSort_Thread( long <Static_Array> )
 *  void MiniBlockSort_Thread( long <Static_Array>, bool IsAscending )
 *
 *  void MiniBlockSort_Thread( long <Dinamic_Array>, size_t nSlots, bool IsAscending )
 *  // void MiniBlockSort_Thread( long <Dinamic_Array>, size_t nLow, size_t nHight, bool IsAscending )
 *
 ***************************************************************************************/
#ifndef __ebm_miniblocksort_thread_h__
#define __ebm_miniblocksort_thread_h__

#include "ebm-miniblocksort.h"

static Return_t  MiniBlockSort_Thread_Asc(  void* p_ptrParam );
static Return_t  MiniBlockSort_Thread_Desc( void* p_ptrParam );

static void Merge_MiniBlockSort_Thread_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nLow2,  size_t p_nStop, 
                                        long p_pBuffer[], size_t p_nBLow ) ;
                      
// static void Merge_MiniBlockSort_Thread_Desc(long p_pArray[],  size_t p_nLow,  size_t p_nLow2,  size_t p_nStop, 
                                         // long p_pBuffer[], size_t p_nBLow ) ;

// static size_t  Search_MinOfMax( long p_nValue, long p_pArray[], size_t p_nLow, size_t p_nHight ) ;

//-------------------------------------------------------------------------

// #define MiniBlockSort_Thread( _Array, ...)     printf("\n\n --> Its work\n");
#define MiniBlockSort_Thread( _Array, ...)     csname2(MiniBlockSort_Thread_, Count_Args(__VA_ARGS__))( (_Array), ##__VA_ARGS__)
#define MiniBlockSort_Thread_0( _Array )                                _MiniBlockSort_Thread( _Array, sizeofarray(_Array), ASC)
#define MiniBlockSort_Thread_1( _Array, _nSlots)                        _MiniBlockSort_Thread( _Array, (_nSlots), ASC )
#define MiniBlockSort_Thread_2( _Array, _nSlots, _bIsAscending )        _MiniBlockSort_Thread( _Array, (_nSlots), (_bIsAscending) )
#define MiniBlockSort_Thread_3( _Array, _nSlots, _Type, _bIsAscending)  _MiniBlockSort_Thread( _Array, (_nSlots), (_bIsAscending) )

Struct(ParamMBS_t) {
    long  *pArray;
    size_t nLow 
         , nHight ;
    long  *pBuffer;
    size_t nBLow 
         , nBHight ;
};


long LimitSlotsInArray_Thread(size_t p_nSlots) {
    if( false ) 
        NULL;
    else if( p_nSlots < 1000) 
        return 500+1 ;
    else if( p_nSlots < 2000) 
        return 1000+1 ;
    else if( p_nSlots < 3000) 
        return 1500+1 ;
    else if( p_nSlots < 5000) 
        return 2500+1 ;
    else if( p_nSlots < 10000) 
        return 5000+1 ;
    else if( p_nSlots < 15000) 
        return 7500+1 ;
    else if( p_nSlots < 20000) 
        return 10000+1 ;
    else if( p_nSlots < 25000) 
        return 12000+1 ;
    else if( p_nSlots < 65600) 
        return 25000+1 ;
    else if(  65600 < p_nSlots) 
        return 65600+1 ; // aprox.: 2^16
    return 25000 + 1 ;
}


long LimitSlotsInArray_Thread2(size_t p_nSlots) {
    // return 5000;
    // return 10000;
    // return 20000;
    // return 25000;
    // return 30000;
    // return 35000;
    // return 40000;
    // return 45000;
    // return 50000;
    // return 55000;
    // return 60000;
    // return 65000;
    // return 65600;  // aprox.: 2^16
    // return 70000;
    // return 75000;
    // return 100000;
    if( false ) 
        NULL;
    else if( p_nSlots < 25000) 
        return 12000+1 ;
    else if( p_nSlots < 65600) 
        return 25000+1 ;
    else if(  65600 < p_nSlots) 
        return 65600+1 ; // aprox.: 2^16
    return 25000 + 1 ;
}

inline  static  void _MiniBlockSort_Thread(long p_pArray[], size_t p_nSlots, bool p_bAscending ) {
    if (p_nSlots < 2) return;
    size_t nSlotsBuffer = Middle( 1, p_nSlots ) ;
    long pBuffer[nSlotsBuffer];
    
    long nTopArryThread ;  
    // printf("\t::::::::::::*[ gnTopArryThread = %d, (%d) ]*:::::::::::\n", nTopArryThread, p_nSlots);
    
    if( p_bAscending ) {
        if( p_nSlots < LimitSlotsInArray_Thread(p_nSlots) ) 
            MiniBlockSort_Asc(p_pArray, 0,     p_nSlots-1, 
                               pBuffer, 0, nSlotsBuffer-1 );
        else {
            ParamMBS_t *pParam = New(ParamMBS_t);
            
            pParam->pArray = p_pArray;      pParam->pBuffer = pBuffer;
            pParam->nLow   = 0;             pParam->nBLow   = 0;
            pParam->nHight = p_nSlots-1;    pParam->nBHight = nSlotsBuffer-1;
            
            MiniBlockSort_Thread_Asc( pParam );
        }
    } else
        NULL;
        // MiniBlockSort_Thread_Desc(p_pArray, 0, p_nSlots-1, 
                          // pBuffer, 0, nSlotsBuffer-1 );
}

// ...............................................

static Return_t  MiniBlockSort_Thread_Asc( void* p_ptrParam ) {
    ParamMBS_t *pParam = (ParamMBS_t*)p_ptrParam ;
    
    long  *pArray  = pParam->pArray,
          *pBuffer = pParam->pBuffer,
          nTmp;

    size_t  nLow    = pParam->nLow,
            nHight  = pParam->nHight,
            nSlots  = nHight - nLow + 1,
            nBLow   = pParam->nBLow,
            nBHight = pParam->nBHight    ;
            
    if( nSlots < 3) {
        if( nSlots == 2 and  pArray[nLow] > pArray[nHight] ) 
            Swap( pArray[nLow], pArray[nHight], nTmp ) ;
        Delete(pParam) ;    //free pParam from Heap.
        Return(0);
    }
    if(nSlots < 17) {
        size_t nStop = nHight + 1;
        for (size_t i = nLow + 1; i < nStop ; ++i) 
            for (size_t j = i
                 ; nLow < j  &&  pArray[j] < pArray[j - 1] 
                 ; --j ) 
                Swap( pArray[j - 1], pArray[j], nTmp );
        Delete(pParam) ;    //free pParam from Heap.
        Return(0);
    }

    size_t  nMidle = Middle( nLow,  nHight ) ,
           nBMidle = Middle( nBLow, nBHight) ;

    if( nSlots < LimitSlotsInArray_Thread2(nSlots) ) {
        MiniBlockSort_Asc(pArray,  nLow,   nMidle, 
                          pBuffer, nBLow, nBMidle );
                          
        MiniBlockSort_Asc(pArray,   nMidle+1, nHight, 
                          pBuffer, nBMidle+1, nBHight );
    } else {  //  ---------[ Preparing Threads]---------
        // printf("\t*************[ Threads in Action ]*************\n");

        ParamMBS_t *pParam1 = New(ParamMBS_t),
                   *pParam2 = New(ParamMBS_t) ;
                   
        pParam1->pArray  = pArray;  pParam1->nLow  = nLow;  pParam1->nHight  = nMidle;   
        pParam1->pBuffer = pBuffer; pParam1->nBLow = nBLow; pParam1->nBHight = nBMidle;
        
        pParam2->pArray  = pArray;  pParam2->nLow  = nMidle+1;  pParam2->nHight  = nHight; 
        pParam2->pBuffer = pBuffer; pParam2->nBLow = nBMidle+1; pParam2->nBHight = nBHight;
        
        Thread( objThread, 2 );     // Create handler for 2 threads.
        
        // New_Threads( objThread, 0, MiniBlockSort_Thread_Asc, pParam1, 0, 100000000 ) ; // Create Thread 1 for MiniBlockSort_Thread_Asc
        // New_Threads( objThread, 1, MiniBlockSort_Thread_Asc, pParam2, 0, 100000000 ) ; // Create Thread 2 for MiniBlockSort_Thread_Asc

        New_Threads( objThread, 0, MiniBlockSort_Thread_Asc, pParam1 ) ; // Create Thread 1 for MiniBlockSort_Thread_Asc
        New_Threads( objThread, 1, MiniBlockSort_Thread_Asc, pParam2 ) ; // Create Thread 2 for MiniBlockSort_Thread_Asc
        
        Wait_Threads( objThread );       // Waits for all threads finished
        Close_Threads( objThread );      // close handle of each thread
        
        Delete(pParam) ;                 // free pParam create from paren process.
    }
    
    // Merge_MiniBlockSort_Thread_Asc(pArray,  nLow, nMidle+1, nHight+1, 
                                  // pBuffer, nBLow );
    Merge_MiniBlockSort_Asc(pArray,  nLow, nMidle+1, nHight+1, 
                                  pBuffer, nBLow );
    Return(0) ;
}
// ...............................................
/* //  This method is not neccesary, because call 'Merge_MiniBlockSort_Asc()' in 'ebm_miniblocksort.h'

static void Merge_MiniBlockSort_Thread_Asc(long p_pArray[],  size_t p_nLow1, size_t p_nLow2,  size_t p_nStop, 
                               long p_pBuffer[], size_t p_nBLow  ) {
    
    size_t nMidle,
           nHight1 = p_nLow2-1;
    long nValue = p_pArray[p_nLow2];

    // Averiguar desde que posición del Slice 1 se  mueven al Buffer.
    if( nValue < p_pArray[p_nLow1] ) 
        NULL;
    else if( p_pArray[nHight1] <= nValue ) 
        p_nLow1 = nHight1 + 1;
    else for(;;) {
        nMidle = Middle( p_nLow1, nHight1) ;
        if( nValue < p_pArray[nMidle] ) 
            nHight1 = nMidle ;
        else {
            p_nLow1 = nMidle+1 ;
            if( nValue < p_pArray[p_nLow1] ) break ;
        }
        if( p_nLow1 == nHight1 ) {
            p_nLow1 += (nValue < p_pArray[p_nLow1] ? 0 : 1) ;
            break;
        }
    }
    if( p_nLow1 == p_nLow2 ) return ;   // Perfect case, does nothing.
    // -------------------------------------------------------------
    // Mueve el  Slice 1 = p_pArray[ p_nLow1 .. p_nLow2-1] al Buffer.
    size_t nBStop = p_nBLow,
           pIter1 = p_nLow1,
           nBIter, nIter2 ;
    while( pIter1 < p_nLow2 )   
        p_pBuffer[nBStop++] = p_pArray[pIter1++] ;
    
    pIter1 = p_nLow1 ;
    nIter2 = p_nLow2 ;
    nBIter = p_nBLow ;
    while( nBIter < nBStop and nIter2 < p_nStop ) 
        p_pArray[pIter1++] = ( p_pBuffer[nBIter] < p_pArray[nIter2] )
                            ? p_pBuffer[nBIter++]
                            :  p_pArray[nIter2++] ;
    
    while( nBIter < nBStop ) 
        p_pArray[pIter1++] = p_pBuffer[nBIter++] ;
}
/**/

//-------------------------------------------------------------------------
#endif // #ifndef __ebm_miniblocksort_thread_h__
