/********************************************************************************************************
 * @Objetive : Provide access to THREADS to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             functions to manipulate memroy load the standard header for
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************
 *  > Control Structure  for threads:
 *    ------------------------------
 *      typedef struct  {
 *          DWORD   dwId;
 *          HANDLE  hThread;
 *      } Thread_t ;                 //  Structure for control one threads.
 *
 *       typedef struct {
 *           DWORD   *anId;
 *           HANDLE  *ahThread;
 *           uint     nThreads;
 *       } Threads_t ;               //  Structure for control multiples-threads.
 *
 *  > Instruction to Define, Redefine and Close Threads control variables:
 *    -------------------------------------------------------------------
 *      Thread(                      // Define a new Control Simple Thread variable.
 *             ObjHandle             // New Control Simple Thread variable.
 *            )                      
 *                                   
 *      Thread(                      // Define a new Control Multi-Threads variable.
 *               ObjArrHandle        // New Control Multi-Threads variable.
 *             , nThreads            // Quantity of Thread to create.
 *            )                      
 *                                   
 *      ReThread(                    // Redefine a existing Control Multi-Threads variable.
 *                  ObjArrHandle,    // Redefine size of Control Single/Multi-Threads variable.
 *               [, nThreads]        // Quantity of Thread to create.
 *              )
 *
 *      Close_Thread(                // Close the Control Simple Thread variable.
 *                    ObjThread      // Control Simple Thread variable, created by Thread( ObjThread ).
 *                  )
 *
 *      Close_Threads(               // Close the Control Multi-Threads variable
 *                    ObjThreads     // Control Multi-Threads variable, created by Thread( ObjThreads, N ).
 *                   )
 *
 *  > Function to launch and wait for threads:
 *    ---------------------------------------
 *      New_Thread(                  // Launch one thread
 *                   ObjThread       // Control Simple Thread variable, create with New_Thread().
 *                  , fnFuntor       // function with the form:  DWORD WINAPI fnFuntor(LPVOID lpParam)
 *                 [, PtrObjParam    // (Default is NULL) Pointer a variable or structure with parameters values for the function.
 *                 [, nMode          // (Default is 0)    Mode to launch the thread: 0: execute the function inmediatly; CREATE_SUSPENDED, STACK_SIZE_PARAM_IS_A_RESERVATION
 *                 [, nSizeStack     // (Default is 0)   Indicate size of stack for execute the thread, values: 0, STACK_SIZE_PARAM_IS_A_RESERVATION,
 *                 [, ptrAttrib]     // (Default is NULL) Pointer a structure with security attributes.
 *                   ] ] ]
 *                )
 *
 *      New_Threads(                 // Launch multiples threads.
 *                     ObjThreads    // Control Multi-Threads variable, created with: New_Threads().
 *                   , nIndex        // Counter for thread launched. NOTE: the first launch must star in zero and increment in one in each launch.
 *                   , fnFuntor      // function with the form:  DWORD WINAPI fnFuntor(LPVOID lpParam)
 *                  [, PtrObjParam   // (Default is NULL) Pointer a variable or structure with parameters values for the function.
 *                  [, nMode         // (Default is 0)    Mode to launch the thread: 0: execute the function inmediatly; CREATE_SUSPENDED, STACK_SIZE_PARAM_IS_A_RESERVATION
 *                  [, nSizeStack    // (Default is 0)    Indicate size of stack for execute the thread, values: 0, STACK_SIZE_PARAM_IS_A_RESERVATION,
 *                  [, ptrAttrib]    // (Default is NULL) Pointer a structure with security attributes.
 *                    ] ] ]
 *                 )
 *
 *      Wait_Thread(                     // Wait for a simple thread, launced with Thread().
 *                   ObjThread          // Control Simple Thread variable
 *                 [, nTimeInMillisec]  // (Default is INFINITE) Time in milli-seconds to wait or force to terminate a thread.
 *                 )
 *
 *      Wait_Threads(                    // Wait for multiples-threads.
 *                    ObjThreads        // Control Multi-Threads variable, launched with Threads().
 *                  [, bWaitForAll      // (Default TRUE) Boolean flag: TRUE wait for all thread launched, FALSE only wait for the first thread in ending.
 *                  [, nTimeInMillisec] // (Default is INFINITE) Time in milli-seconds to wait or force to terminate all threads.
 *                      ]
 *                  )
 *
 *  > Function to suspend and resume threads:
 *    --------------------------------------
 *      Suspend_Thread(                 // Suspend execution of a thread.
 *                       ObjThread      // Control Simple/Multiple Thread variable.
 *                    [, nIdxObject]    // Index identifier for Control Multiple Thread variable.
 *                    )
 *
 *      Resume_Thread(                  // Resume execution of a thread.
 *                      ObjThreads      // Control Simple/Multiple Thread variable.
 *                    [, nIdxObject]    // Index identifier for Control Multiple Thread variable.
 *                   )
 *
 *
 *  Functions to use inside Thread context
 *  ======================================
 *
 *  > Terminate a Thread:
 *    ------------------
 *      Return(                         // End normal execution of function thread, used it inside of thread.
 *             [nExitCode=0]            // Exit Code from thread, restriction: nExitCode >= 0
 *            )
 *
 *      Exit_Thread(                    // Suicide Thread, forced execution of thread and thread sons . used it inside of thread.
 *                  [nExitCode=0]       // Exit Code from thread, restriction: nExitCode >= 0
 *                 )
 *
 *
 *      Terminate_Thread(               // End execution of thread. used it outside of thread.
 *                        ObjThread     // Control Simple/Multiple Thread variable.
 *                      , nExitCode     // Exit Code from thread, restriction: nExitCode >= 0
 *                      )
 *
 *  > Get Information:
 *    ---------------
 *      ulong     Get_ProcessesId()     // Get Proccess Identifier
 *
 *      handler_t Get_Processes()       // Get handerl for current procces ('HANDLE' in windows; 'int' in unix family)
 *
 *      handler_t Get_Thread()          // Get handerl for current procces ('HANDLE' in windows; 'thread_t' in unix family)
 *
 *  Functions to use Outside Thread context
 *  =======================================
 *
 *  > Get information about Thread:
 *    ----------------------------
 *       Returned(              // Show value retruned by function thread when it terminate thread.
 *                  ObjThread   // Control Simple/Multiple Thread variable.
 *                 [, nIndex]   // Index about Control variable.
 *               )  
 *
 *       Exit_Code(             // Idem Returned(), it is an alias.
 *                  ObjThread    
 *                 [, nIndex]   
 *                )  
 *
 *       Returned_Success(      // Get a boolena value if returned succes or forced terminate thread.
 *                  ObjThread   // Control Simple/Multiple Thread variable.
 *                 [, nIndex]   // Index about Control variable.
 *                )  
 *
 *       Id_Thread(             // Get value from function on terminate thread.
 *                  ObjThread   // Control Simple/Multiple Thread variable.
 *                 [, nIndex]   // Index about Control variable.
 *               )  
 *
 *       Handle_Thread(             // Get value from function on terminate thread.
 *                       ObjThread  // Control Simple/Multiple Thread variable.
 *                     [, nIndex]   // Index about Control variable.
 *                    )  
 *
 *
 *********************************************************************************************************
 *
 *  Observations:
 *  ------------
 *  The macro functions New() and Delete() are redefined to use a different model 
 *  memory for threads.
 *
 *  TO DO:
 *  ----- 
 *  - Implement Critical Sections
 *  - Implement Mutex functions
 *  - Implement Semaphores functions
 *  - Implement Process functons
 *  - Implement Network functions.
 *  - Implement comunication functions between threads. 
 *  - Implement Attributes mechanism for threads. 
 *   
 *********************************************************************************************************/
#ifndef __ebm_thread_h__
#define __ebm_thread_h__

#ifdef EBM_THREAD
    #ifdef _WINDOWS_ 
        #include "ebm-thread-win.h"
        
    #elif defined(unix) or defined(__unix) or defined(__unix__) or \
          defined(__FreeBSD__) or defined(__NetBSD__) or defined(__OpenBSD__) or \
          defined(__APPLE__) or defined(__MACH__) or defined(__linux__)
        #include "ebm-thread-unix.h"
    #else
        #include "ebm-thread-nothing.h"
    #endif
#else
    #include "ebm-thread-nothing.h"
#endif  // #ifdef EBM_THREAD

#endif  // #ifndef __ebm_thread_h__
