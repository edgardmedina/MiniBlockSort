/********************************************************************************************************
 * @Objetive : Provide generic functions to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             functions to manipulate memroy load the standard header for
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Medina Q. <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 ********************************************************************************************************
 *  Constants:
 *  ---------
 *
 *  const int  ASC=true;
 *  const int  DESC=false;
 *
 *  Numeric Function
 *  ----------------
 *  int isOdd(  long nNumber )      // Retorna un boolean, verifica si un numero es impar.
 *  int esImpar(  long nNumber )    // Idem  isOdd().
 *  int isEven( long nNumber )      // Retorna un boolean, verifica si un numero es par.
 *  int esPar(  long nNumber )      // Idem isEven()
 *
 *
 *
 * Miscellaneous Template Functions :
 * --------------------------------
 *  <Type> Max2( <Type> a, <Type> b  )  : Get maximun value of 'a' or 'b' number values,
 *
 *  void Swap( <Type> a, <Type> b [, <Type> tmp] )  : Swapped two values, optionally let to use a
 *                                                    a extern temporal variable for best performance
 *                                                    in loops.
 * void Randomize( [size_t Init_Value] )   : Initialize the seed for Random() functions.
 *                                              Init_Value:  {empty}  Generate a random value from system clock.
 *                                                            n < 0   Generate a random value from system clock.
 *                                                            0 <= n  Generate a random value from 'n' base number.
 *
 * int  Random( [size_t nTop_Value] )      : Generate random number. optionally you can specified a top value for random values.
 *
 *
 * Array Functions:
 * ----------------
 *  * Check out if array ir sorted in ascendig mode or descending mode.
 *     bool  IsArraySorted( long* Array, int nSlotsArray, bool isAscending) :
 *     void  Check_Sorted(  long* Array, int nSlotsArray, bool isAscending) :
 *
 *  * Template procedure to show an array on screen
 *     void  show_array( <Type>, const char* strMask, <type>* Array, size_t nSlotsArray )
 *
 *  * Function get quantity of slots in Static Array. NOTE: don't work in Dynamic Arrays.
 *     int sizeofarray( Static_Array )
 *     int arraysize( Static_Array )
 *
 * ------------------------------------------------------------------------------------
 *
 *  Private Macros To Create Function Templates in 'C'
 * ---------------------------------------------------
 *
 *  This macros let me create a limited style of overload of functions to agregate most power
 *  to C programing
 *
 *  * Template Procedure:
 *        #define  <MacroName>( <Argument_list> )\
 *            Procedure\
 *               ... \   // use here the arguments
 *            EndProcedure
 *
 *  * Create a C Command:
 *        csname2(  token1, token2 )  : joint two tokens to form a single name of command (macro, function, variable or whatever).
 *
 *  * Macro to count quantity of Variadic Argument. Support from 0 until 62 arguments, it returns a LITERAL Number
 *       size_t Count_Args( __VA_ARGS__ )
 *
 ********************************************************************************************************/
// #define TEMPLATE
#ifndef __ebmq__h__
#define __ebmq__h__

/*******************************************************************************
 * Prepare Operating System APIs
 *******************************************************************************/
#if defined(_WIN32) || defined(_WIN64)
    #define _WIN_FAMILY_
#endif
#if defined(__MINGW32__) && defined(__MINGW64__) 
    #ifndef __MINGW__
        #define __MINGW__
    #endif
#endif

#if  defined(__linux__) || defined(unix) || defined(__unix) || defined(__unix__) || \
     defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #define _UNIX_FAMILY_
#endif

#if defined(_WIN_FAMILY_) 
    #if !defined(_WINDOWS_) 
        #include <windows.h> // 
    #endif
    #ifdef __MINGW__
        #include <unistd.h>
       // #ifdef EBM_THREAD
            // #include <pthread.h>
       // #endif
    #else
        typedef long clockid_t ;
        static BOOL _g_bFirst_Time = 1;
        static LARGE_INTEGER _g_nCounts_per_Sec;
        const clockid_t CLOCK_MONOTONIC = 0;
    #endif
#endif

#ifdef _UNIX_FAMILY_
   #include <sys/unistd.h>
   #ifdef EBM_THREAD
        #include <pthread.h>
   #endif
#endif

/*****************************************************************************/
// With this include header is not necessary add 'extern C {' and '}' at beggining and end of this file
// and another header files.
#ifdef __cplusplus
    #include <cstring>
    #include <cstddef>
    #include <cstdlib>
    #include <cstdio>
    #include <ctime>
    #include <cfloat>
    #include <cstring>
    #include <cmath>
#else
    #include <string.h>
    #include <stdarg.h>
    #include <stddef.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <time.h>
    #include <float.h>
    #include <string.h>
    #include <math.h>
#endif

#include <sys/timeb.h>
#include <sys/time.h>


/**************************************************************************
* This sections is critical, don't change the order in the invocation 
* of 'ebm-xxx.h' library.
***************************************************************************/
#include "ebm-types.h"
#include "ebm-base.h"
#include "ebm-alloc.h"
#include "ebm-thread.h"

 /// No critical order.
#include "ebm-random.h"
#include "ebm-sort.h"
#include "ebm-time.h"

/**************************************************************************
 * Here is here and go on the invocation is free
 ***************************************************************************/

#include "ebm-sort-test.h"

#endif // end #define __ebmq__h__
