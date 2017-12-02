/*********************************************************************************************
 * @Algorith   : Flashsort
 *               Generic adaptation in 'C' + EBM Framework, to works in 'C' and/or C++ from:
 *                 Source        : Flashsort.f95
 *                 @author       : Karl-Dietrich Neubert
 *                 @version      : Copyright (c) K. - D. NEUBERT  1997.
 *                 @last modified: 09 Jan 1998
 *
 * @Adapted by : Edgard Medina Q. <edgardmedina@yahoo.com>, Arequipa, Perú
 *
 * @Observation: This algorith is generic and minimun requiere ISO C99 or C++98,
 *               suport native types or custom C/C++ Structures (variables) or C++ Classes (objects).
 *
 *               To work correctly is necessary define a Funtor compare with prototype:
 *                   int (*fnCompare)(const void*, const void*);
 *
 *               NOTE: in C++, the funtor must be declared as 'friend function' to Class.
 *
 *********************************************************************************************/

#include "../../ebm/library.h"
#include "../../ebm/io.h"

//-----------------------------------------------------
void Flashsort_Asc ( long* A, int nA );
void Flashsort_Desc( long* A, int nA );
//-----------------------------------------------------

inline void Flashsort_ebm (   // Procedure: Sort an Array.
     long* A                  //     Param: Array to sort of type long.
    ,int   nA                 //     Param: Size of Array.
    , bool p_IsAscending      //     Param: Flag to sor the array, 'true' for ascending and 'false' for descending
) {                           //   Returns: Array 'A' sorted.
   If p_IsAscending then
      Flashsort_Asc(A, nA);
   else
      Flashsort_Desc(A, nA);
}

//***********************************************************

void Flashsort_Asc (      // Procedure: Sort an Array.
     long* A       //     Param: Array to sort of type long.
    ,int   nA      //     Param: Size of Array.
) {                //   Returns: Array 'A' sorted.
   // Sorts array A with nA elements by use of index vector L 
   // of dimension nL with nL about 0.1 nA. 
   // Copyright (C)  k. - D. Neubert  1997                           
   If nA < 3  Then
      fprintf(stderr, "Error FlashSort: The array size must be greater than 2 'nL' .\n");
      return;
   End
   int i = 0, j = 0, k = 0, nL;
   long vHold  ,
        vFlash ; 
   If      nA < 20  Then nL = 3 ;
   ElseIf  nA < 40  Then nL = 4 ;
   ElseIf  nA < 60  Then nL = 5 ;
   ElseIf  nA < 100 Then nL = 10 ;
   ElseIf  nA < 150 Then nL = 15 ;
   Else nL = 20 ;
   End
   nL = nA / nL ;
   //============================ CLASS FORMATION ===== 
   long vMin = A[0];
   int  iMax = 0 ;
   
   Foreach( i, inLoop, 1, nA-1 )Do
      If A[i] < vMin    then vMin = A[i] ;
      If A[i] > A[iMax] then iMax = i ;
   EndForeach
   
   If vMin == A[iMax] then return;
   
   double c1 = ( (double)nL - 1.0) / (A[iMax] - vMin) ;
   int*   L  =  mem_newz(int, nL) ;
   
   Foreach( i, inLoop, 0, nA-1 )Do
      k = (int)(c1 * (A[i] - vMin) ) ;
      ++L[k] ;
   EndForeach
   
   Foreach( k, inLoop, 1, nL-1 )Do
      L[k] += L[k - 1] ;
   EndForeach
   
   Swap_c( A[0], A[iMax], vHold);
   
   //=============================== PERMUTATION ===== 
   int nMove = 0;
   j = 0 ;
   k = nL - 1 ;
   While nMove < nA - 2 Do
      While j > L[k] - 1 Do 
         ++j ;
         k = (int)( c1 * (A[j] - vMin ) ) ;
      End 
      vFlash = A[j] ;
      While j != L[k] Do 
         k = (int)(c1 * (vFlash - vMin)) ;
         Swap_c( vFlash, A[L[k]-1], vHold );
         --L[k] ;
         ++nMove ;
      End
   End
   mem_delete( L ) ;
   //========================= STRAIGHT INSERTION =====
   Foreach( i, inLoop, (nA-1)-2, 0 ) Do
      If A[i + 1] < A[i] Then
         vHold = A[i] ;
         j = i ;
         While A[j + 1] < vHold Do
            A[j] = A[j + 1] ; 
            ++j ;
         End
         A[j] = vHold ;
      End
   EndForeach
   //=========================== END FLASH1 =====
}
//***********************************************************

void Flashsort_Desc (      // Procedure: Sort an Array.
     long* A       //     Param: Array to sort of type long.
    ,int   nA      //     Param: Size of Array.
) {                //   Returns: Array 'A' sorted.
   // Sorts array A with nA elements by use of index vector L 
   // of dimension nL with nL about 0.1 nA. 
   // Copyright (C)  k. - D. Neubert  1997                           
   If nA < 3  Then
      fprintf(stderr, "Error FlashSort: The array size must be greater than 2 'nL' .\n");
      return;
   End
   int i = 0, j = 0, k = 0, nL;
   long vHold, vFlash ; 
   If      nA < 20  Then nL = 3 ;
   ElseIf  nA < 40  Then nL = 4 ;
   ElseIf  nA < 60  Then nL = 5 ;
   ElseIf  nA < 100 Then nL = 10 ;
   ElseIf  nA < 150 Then nL = 15 ;
   Else nL = 20 ;
   End
   nL = nA / nL ;
   //============================ CLASS FORMATION ===== 
   long vMax = A[nA-1];
   int  iMin = 0 ;
   
   Foreach( i, inLoop,  nA-2, 0 )Do
      If A[i] > vMax    then vMax = A[i] ;
      If A[i] < A[iMin] then iMin = i ;
   EndForeach
   
   If vMax == A[iMin] then return;
   
   double c1 = ( (double)nL - 1.0) / (A[iMin] - vMax) ;
   int*   L  =  mem_newz(int, nL) ;
   
   Foreach( i, inLoop, nA-1, 0 )Do
      k = (int)(c1 * (A[i] - vMax) ) ;
      ++L[k] ;
   EndForeach
   
   Foreach( k, inLoop, nL-2 , 0 )Do
      L[k] += L[k + 1] ;
   EndForeach
   
   Swap_c( A[0], A[iMin], vHold);
   
   //=============================== PERMUTATION ===== 
   int nMove = 0;
   j = 0 ;
   k = nL - 1 ;
   While nMove < nA - 2 Do
      While j > L[k] - 1 Do 
         ++j ;
         k = (int)( c1 * (A[j] - vMax ) ) ;
      End 
      vFlash = A[j] ;
      While j != L[k] Do 
         k = (int)(c1 * (vFlash - vMax)) ;
         Swap_c( vFlash, A[L[k]-1], vHold );
         --L[k] ;
         ++nMove ;
      End
   End
   mem_delete( L ) ;
   //========================= STRAIGHT INSERTION =====
   Foreach( i, inLoop, (nA-1)-2, 0 ) Do
      If A[i + 1] < A[i] Then
         vHold = A[i] ;
         j = i ;
         While A[j + 1] < vHold Do
            A[j] = A[j + 1] ; 
            ++j ;
         End
         A[j] = vHold ;
      End
   EndForeach
   //=========================== END FLASH1 =====
}

//--------------------------------------------------------------------

void main(int nArg, char** Args) {
   long Arr[] = {42, 26, 16, 10, 21, 63, 3, 46, 41, 7, 5 };
   long Arr2[] = {42, 26, 16, 10, 21, 63, 3, 46, 41, 7, 5 };
   int nA = sizeofarray(Arr);

   writeln("---------------------------------");
   show_array(i, Arr,nA);
   show_proc( Flashsort_ebm( Arr, nA, true ) );
   show_array(i, Arr,nA);

   writeln("---------------------------------");
   show_array(i, Arr,nA);
   show_proc( Flashsort_ebm( Arr, nA, false ) );
   show_array(i, Arr,nA);

   writeln("---------------------------------");

   return 0;
}



