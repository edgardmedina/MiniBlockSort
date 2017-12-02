/***************************************************************************************
 * @Objetive : Test the "Mergesort" Algorithm.
 * @version  : v1.0, Copyleft 2011.
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 ***************************************************************************************/
 
#include "ebm/ebm.h"
#include "sorts/shellsort.h"

int main(int nArg, char** Args) {

    bool bShow_Array = false ;
    // bool bShow_Array = true ;

   // int nSlots = 1500000 ; // maximun support by Tiny C.
   // int nSlots = 1000000 ; // maximun support by Tiny C.
   // int nSlots = 921921 ; // maximun support by Tiny C.
   // int nSlots = 721921 ; // maximun support by Tiny C.
   int nSlots = 521921 ; // maximun support by Tiny C.
   // int nSlots = 261021 ; // maximun support by Tiny C.
   // int nSlots = 520964 ;// maximun support by Tiny C (ok).
   // int nSlots = 26 ; // maximun support by Tiny C.
   // int nSlots = 100 ; 
   long *Array = New( long, nSlots ) ;
   bool TypeOrder;
   llong nStartTime,
         nEndTime,
         nElapsed;
   
   
   printf("\n-------------------[ ShellSort Ascending / %d ]--------------------\n", nSlots);

   Randomize(0);
   for( int i=0; i< nSlots ; ++i ) 
      Array[i] = Random(long, 300000);  // Advanced Random Function
      // Array[i] = Rand(30000);           // Basic Random Function
   
   TypeOrder = ASC ;
   if( bShow_Array ) show_array(long, "%d", Array, nSlots);
   Check_Sorted(long, Array, nSlots, TypeOrder) ;
   
   nStartTime = gettime_nsec();
   ShellSort( Array, nSlots, TypeOrder ) ;
   nEndTime = gettime_nsec();
   nElapsed = Elapsed( nStartTime, nEndTime);
   
   if( bShow_Array ) show_array(long, "%d", Array, nSlots);
   Check_Sorted(long, Array, nSlots, TypeOrder) ;
   printf("    Time elapsed: %f ms\n", nsec2msec(nElapsed) );
   /**/
   printf("\n-------------------[ SellSort2 Ascending / %d ]--------------------\n", nSlots);

   Randomize(0);
   for( int i=0; i< nSlots ; ++i ) 
      Array[i] = Random(long, 300000);  // Advanced Random Function
      // Array[i] = Rand(30000);           // Basic Random Function
   
   TypeOrder = ASC ;
   if( bShow_Array ) show_array(long, "%d", Array, nSlots);
   Check_Sorted(long, Array, nSlots, TypeOrder) ;
   
   nStartTime = gettime_nsec();
   SellSort2( Array, nSlots );
   nEndTime = gettime_nsec();
   nElapsed = Elapsed( nStartTime, nEndTime);
   
   if( bShow_Array ) show_array(long, "%d", Array, nSlots);
   Check_Sorted(long, Array, nSlots, TypeOrder) ;
   printf("    Time elapsed: %f ms\n", nsec2msec(nElapsed) );
   
   printf("\n---------------------------------------------------------\n");
   Delete( Array, 0 ) ; 
   
   return 0;
}
