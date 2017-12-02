/***************************************************************************************
 * @Algorithm : MiniBlock Sort.
 * @Objetive  : Sort algorithm for generic data, is stable.
 * @version   : v1.0, Copyleft 2012 - Sep/2017
 * @modified  : October/2017
 * @Author    : Edgard Medina Q. <edgardmedina@yahoo.com>
 *              Arequipa, Perú
 ***************************************************************************************
 * Syntax:
 *  void MiniBlockSort( long <Static_Array> )
 *  void MiniBlockSort( long <Static_Array>, bool IsAscending )
 *
 *  void MiniBlockSort( long <Dinamic_Array>, size_t nSlots, bool IsAscending )
 *  // void MiniBlockSort( long <Dinamic_Array>, size_t nLow, size_t nHigh, bool IsAscending )
 *
 ***************************************************************************************/
#ifndef __ebm_miniblocksort2_h__
#define __ebm_miniblocksort2_h__

#include "ebm-sort.h"

static void MiniBlockSort2_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nHigh, 
                              long p_pBuffer[], size_t p_nBLow, size_t p_nBHigh );
                
static void MiniBlockSort2_Desc(long p_pArray[],  size_t p_nLow,  size_t p_nHigh, 
                                long p_pBuffer[], size_t p_nBLow, size_t p_nBHigh );

static void Merge_MiniBlockSort2_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nLow2,  size_t p_nStop, 
                                     long p_pBuffer[], size_t p_nBLow ) ;
                      
static void Merge_MiniBlockSort2_Desc(long p_pArray[],  size_t p_nLow,  size_t p_nLow2,  size_t p_nStop, 
                                     long p_pBuffer[], size_t p_nBLow ) ;

//-------------------------------------------------------------------------

// #define MiniBlockSort( _Array, ...)     printf("\n\n --> Its work\n");
#define MiniBlockSort2( _Array, ...)     csname2(MiniBlockSort2_, Count_Args(__VA_ARGS__))( (_Array), ##__VA_ARGS__)
#define MiniBlockSort2_0( _Array )                                _MiniBlockSort2( _Array, sizeofarray(_Array), ASC)
#define MiniBlockSort2_1( _Array, _nSlots)                        _MiniBlockSort2( _Array, (_nSlots), ASC )
#define MiniBlockSort2_2( _Array, _nSlots, _bIsAscending )        _MiniBlockSort2( _Array, (_nSlots), (_bIsAscending) )
#define MiniBlockSort2_3( _Array, _nSlots, _Type, _bIsAscending)  _MiniBlockSort2( _Array, (_nSlots), (_bIsAscending) )


inline  static  void _MiniBlockSort2(long p_pArray[], size_t p_nSlots, bool p_bAscending ) {
   if (p_nSlots < 2) return;
   size_t nSlotsBuffer = Middle( 1, p_nSlots) ;
   long pBuffer[nSlotsBuffer];
   
   if( p_bAscending ) 
      MiniBlockSort2_Asc( p_pArray, 0, p_nSlots-1, 
                          pBuffer, 0, nSlotsBuffer-1 );
   // else
      // MiniBlockSort2_Desc(p_pArray, 0, p_nSlots-1, 
                          // pBuffer, 0, nSlotsBuffer-1 );
}

// ...............................................

static void MiniBlockSort2_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nHigh, 
                               long p_pBuffer[], size_t p_nBLow, size_t p_nBHigh ) {
    size_t nSlots = p_nHigh - p_nLow + 1 ;
    
    /*
    if( nSlots < 17 ) {
        if( nSlots < 2 ) return ;
        long nTmp;
        size_t i ,
               nStop ,
               nOdd = isOdd(nSlots) ;
        for(  i=0, nStop = nSlots - nOdd; i < nStop ; i += 2 ) 
            if( p_pArray[i] > p_pArray[i+1] ) 
                Swap(p_pArray[i], p_pArray[i+1], nTmp )
        if( nOdd ) {
            if( p_pArray[i] < p_pArray[i-2] )  {
                nTmp = p_pArray[i] ;
                p_pArray[i] = p_pArray[i-1] 
                p_pArray[i-1] = p_pArray[i-2] ; 
                p_pArray[i-2] = nTmp ; 
            } else if( p_pArray[i] < p_pArray[i-1] ) 
                Swap( p_pArray[i], p_pArray[i-1], nTmp)
        }
        if( nSlots < 4 ) return ;
        size_t nBlocks = nSlots / 4 - 1,
               nRemain = nSlots % 4
        for( i =0 ; i < nStop ; i += 4 ) { 
            for( size_t j = i, nStop2 = nBlocks*4 ; i < nStop2 ; ++j ) {
                if( p_pArray[j+2] < p_pArray[j]  ) 
                    Merge_MiniBlockSort2_Asc( p_pArray,  j, j+2,  size_t j+4, 
                                              p_pBuffer, p_nBLow  ) ;
                if( p_pArray[j+2] < p_pArray[j+1]  ) 
                    Merge_MiniBlockSort2_Asc( p_pArray,  j+1, j+2,  size_t j+4, 
                                              p_pBuffer, p_nBLow  ) ;
                // if( p_pArray[i] > p_pArray[i+1] ) Swap(p_pArray[i], p_pArray[i+1], nTmp )
            }
        }
        if( nRemain > 0 ) {
            nlow = nBlocks*4
            nlow2 = nlow+4 ;
            nstop = p_nBHigh + 1
        }
    }
    /**/
    
// /*    
    if( nSlots < 3) {
        if( nSlots == 2 and  p_pArray[p_nLow] > p_pArray[p_nHigh] ) 
            Swap( p_pArray[p_nLow], p_pArray[p_nHigh], p_pBuffer[p_nBLow] ) ;
        return ;
    }
    // if(nSlots < 17) {
       // size_t nStop = p_nHigh + 1;
       // long nTmp;
       // for (size_t i = p_nLow + 1; i < nStop ; ++i) 
          // for (size_t j = i
              // ; p_nLow < j  &&  p_pArray[j] < p_pArray[j - 1] 
              // ; --j ) 
             // Swap( p_pArray[j - 1], p_pArray[j], nTmp );
        // return;
    // }
/**/
    // Consider (p_nLow+p_nHigh)/2 == p_nLow+(p_nHigh- p_nLow)/2, this avoids 
    // overflow for large nFirst and p_nHigh values.
    size_t  nMidle = Middle( p_nLow,  p_nHigh), //  nMidle =  p_nLow + ( p_nHigh -  p_nLow) / 2,
           nBMidle = Middle(p_nBLow, p_nBHigh); // nBMidle = p_nBLow + (p_nBHigh - p_nBLow) / 2;

    // Sort the first and the second halves/slices
    MiniBlockSort2_Asc(p_pArray,  p_nLow,   nMidle, 
                      p_pBuffer, p_nBLow, nBMidle );
                      
    MiniBlockSort2_Asc(p_pArray,   nMidle+1, p_nHigh, 
                      p_pBuffer, nBMidle+1, p_nBHigh );

    // ...........................
    p_nLow = MinOfMax_Search( p_pArray[nMidle+1], p_pArray, p_nLow,   nMidle   ) ;
  
    if( p_nLow > nMidle ) return ;   // Perfect case, does nothing.
    size_t nHight = MinOfMax_Search( p_pArray[nMidle],   p_pArray, nMidle+1, p_nBHigh ) ;
    nHight += p_nBHigh < nHight ? 0 : 1 ;
    
    // ...........................

    Merge_MiniBlockSort2_Asc(p_pArray,  p_nLow,   nMidle+1, p_nHigh+1, 
                             p_pBuffer, p_nBLow);
}
// ...............................................


static inline void Merge_MiniBlockSort2_Asc(long p_pArray[],  size_t p_nLow1, size_t p_nLow2,  size_t p_nStop, 
                                     long p_pBuffer[], size_t p_nBLow  ) {
    // Mueve el  Slice 1: p_pArray[ p_nLow1 .. p_nLow2-1] al Buffer.
    size_t pIter1 = p_nLow1 ,
           nIter2 = p_nLow2 ,
           nBIter = p_nBLow ,
           nBStop = p_nBLow ;
           
    while( pIter1 < p_nLow2 )   
        p_pBuffer[nBStop++] = p_pArray[pIter1++] ;
    
    pIter1 = p_nLow1 ;
    while( nBIter < nBStop and nIter2 < p_nStop ) 
        p_pArray[pIter1++] = ( p_pBuffer[nBIter] < p_pArray[nIter2] )
                            ? p_pBuffer[nBIter++]
                            :  p_pArray[nIter2++] ;
    
    while( nBIter < nBStop ) 
        p_pArray[pIter1++] = p_pBuffer[nBIter++] ;
}



// ...............................................

#endif // #ifndef __ebm_miniblocksort2_h__
