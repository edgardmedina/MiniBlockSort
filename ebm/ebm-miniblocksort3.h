/***************************************************************************************
 * @Algorithm : MiniBlock Sort.
 * @Objetive  : Sort algorithm for generic data, is stable.
 * @version   : v1.0, Copyleft 2012 - Sep/2017
 * @modified  : October/2017
 * @Author    : Edgard Medina Q. <edgardmedina@yahoo.com>
 *              Arequipa, Perú
 ***************************************************************************************
 * Syntax:
 *  void MiniBlockSort3( long <Static_Array> )
 *  void MiniBlockSort3( long <Static_Array>, bool IsAscending )
 *
 *  void MiniBlockSort3( long <Dinamic_Array>, size_t nSlots, bool IsAscending )
 *  // void MiniBlockSort3( long <Dinamic_Array>, size_t nLow, size_t nHigh, bool IsAscending )
 *
 ***************************************************************************************/
#ifndef __ebm_miniblocksort3_h__
#define __ebm_miniblocksort3_h__

#include "ebm-sort.h"

static void MiniBlockSort3_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nHigh, 
                              long p_pBuffer[], size_t p_nBLow, size_t p_nBHigh );
                
static void MiniBlockSort3_Desc(long p_pArray[],  size_t p_nLow,  size_t p_nHigh, 
                               long p_pBuffer[], size_t p_nBLow, size_t p_nBHigh );

static void Merge_MiniBlockSort3_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nLow2,  size_t p_nStop, 
                                    long p_pBuffer[], size_t p_nBLow ) ;
                      
static void Merge_MiniBlockSort3_Desc(long p_pArray[],  size_t p_nLow,  size_t p_nLow2,  size_t p_nStop, 
                                     long p_pBuffer[], size_t p_nBLow ) ;

//-------------------------------------------------------------------------

// #define MiniBlockSort3( _Array, ...)     printf("\n\n --> Its work\n");
#define MiniBlockSort3( _Array, ...)     csname2(MiniBlockSort3_, Count_Args(__VA_ARGS__))( (_Array), ##__VA_ARGS__)
#define MiniBlockSort3_0( _Array )                                _MiniBlockSort3( _Array, sizeofarray(_Array), ASC)
#define MiniBlockSort3_1( _Array, _nSlots)                        _MiniBlockSort3( _Array, (_nSlots), ASC )
#define MiniBlockSort3_2( _Array, _nSlots, _bIsAscending )        _MiniBlockSort3( _Array, (_nSlots), (_bIsAscending) )
#define MiniBlockSort3_3( _Array, _nSlots, _Type, _bIsAscending)  _MiniBlockSort3( _Array, (_nSlots), (_bIsAscending) )


inline  static  void _MiniBlockSort3(long p_pArray[], size_t p_nSlots, bool p_bAscending ) {
   if (p_nSlots < 2) return;
   size_t nSlotsBuffer = Middle( 1, p_nSlots) ;
   long pBuffer[nSlotsBuffer];
   
   if( p_bAscending ) 
      MiniBlockSort3_Asc( p_pArray, 0, p_nSlots-1, 
                          pBuffer, 0, nSlotsBuffer-1 );
   // else
      // MiniBlockSort3_Desc(p_pArray, 0, p_nSlots-1, 
                          // pBuffer, 0, nSlotsBuffer-1 );
}

// ...............................................

static void MiniBlockSort3_Asc(long p_pArray[],  size_t p_nLow,  size_t p_nHigh, 
                              long p_pBuffer[], size_t p_nBLow, size_t p_nBHigh ) {
    size_t nSlots = p_nHigh - p_nLow + 1 ;
    if( nSlots < 3) {
        if( nSlots == 2 and  p_pArray[p_nLow] > p_pArray[p_nHigh] ) 
            Swap( p_pArray[p_nLow], p_pArray[p_nHigh], p_pBuffer[p_nBLow] ) ;
        return ;
    }
    if(nSlots < 17) {
       size_t nStop = p_nHigh + 1;
       long nTmp;
       for (size_t i = p_nLow + 1; i < nStop ; ++i) 
          for (size_t j = i
              ; p_nLow < j  &&  p_pArray[j] < p_pArray[j - 1] 
              ; --j ) 
             Swap( p_pArray[j - 1], p_pArray[j], nTmp );
        return;
    }

    // Consider (p_nLow+p_nHigh)/2 == p_nLow+(p_nHigh- p_nLow)/2, this avoids 
    // overflow for large nFirst and p_nHigh values.
    size_t nMiddle = Middle( p_nLow,  p_nHigh), //  nMiddle =  p_nLow + ( p_nHigh -  p_nLow) / 2,
           nBMidle = Middle(p_nBLow, p_nBHigh); // nBMidle = p_nBLow + (p_nBHigh - p_nBLow) / 2;

    // Sort the first and the second halves/slices
    MiniBlockSort3_Asc(p_pArray,  p_nLow,   nMiddle, 
                      p_pBuffer, p_nBLow, nBMidle );
                      
    MiniBlockSort3_Asc(p_pArray,   nMiddle+1, p_nHigh, 
                      p_pBuffer, nBMidle+1, p_nBHigh );

    p_nLow = MinOfMax_Search( p_pArray[nMiddle+1], p_pArray, p_nLow, nMiddle ) ;
    if( p_nLow > nMiddle ) 
        return ;
    
    Merge_MiniBlockSort3_Asc(p_pArray,  p_nLow,   nMiddle+1, p_nHigh+1, 
                            p_pBuffer, p_nBLow);
}
// ...............................................


static void Merge_MiniBlockSort3_Asc(long p_pArray[],  size_t p_nLow1, size_t p_nLow2,  size_t p_nStop, 
                                    long p_pBuffer[], size_t p_nBLow  ) {
    // Mueve el  Slice 1 = p_pArray[ p_nLow1 .. p_nLow2-1] al Buffer.
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

#endif      // #ifndef __ebm_miniblocksort3_h__
