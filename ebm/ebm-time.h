/********************************************************************************************************
 * @Objetive : Provide generic functions to works with native C or C++ compilers. The minimun requirement
 *             is ISO C90 or ISO C++94.
 *             functions to manipulate memroy load the standard header for
 * @version  : v1.0, Copyleft 2010 - Sep/2017
 * @Author   : Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>
 *             Arequipa, Perú
 *********************************************************************************************************
 *  USER OBJECTS
 *  ============
 *
 *  Structures for user:
 *  --------------------
 *   tm    : time structure
 *
 *  Evaluate Time Elapsed ( seconds, mili seconds, micro seconds, decimal of nano seconds)
 *  ------------------------------------------------------------------------
 *   <type> Elapsed( <type> nStartTime, <type> nEnd_Time )      // [ Macro Function ]
 *
 *  Evaluate Timestamp Elapsed in micro seconds (usec = 10e-6)  [ Macro Function ]
 *  --------------------------------------------------
 *   char* Timestamp_usec( char* strTime, llong nTime_Elapsed_usec) 
 *   char* Timestamp_usec( char* strTime, llong nBegingettime_usec, llong nStopgettime_usec ) 
 *
 *  Evaluate Timestamp Elapsed in decimals of nano seconds (nsec = 10e10-7)
 *  -------------------------------------------------------------
 *   char* Timestamp_nsec( char* strTime, llong nTime_Elapsed_nsec) 
 *   char* Timestamp_nsec( char* strTime, llong nBeginTime_nsec, llong nStopTime_nsec ) 
 * 
 *  Get time from system
 *  --------------------
 *   llong  gettime_usec()     // Get time in micro seconds (usec=10e-6)
 *   llong  gettime_nsec()     // Get time in decimals of nano seconds (10e-7) 
 *   llong  gettime_nsec2()    // Get time in decimals of nano seconds (10e-7) (Deprecated).
 * 
 ********************************************************************************************************/
#ifndef __ebm_time_h__
#define __ebm_time_h__


#ifndef __cplusplus
typedef struct tm       tm;
typedef struct timeb    timeb;
typedef struct timeval  timeval;
typedef struct timespec timespec;
#endif


    
#ifdef __cplusplus
    extern "C" {
#endif
 
// ------------------------------------------------------------------------

#define nsec2usec( _Timein )   ((_Timein)/1000.0)
#define nsec2msec( _Timein )   ((_Timein)/1000000.0)
#define nsec2sec(  _Timein )   ((_Timein)/1000000000.0)

#define usec2msec( _Timein )   ((_Timein)/1000.0)
#define usec2sec(  _Timein )   ((_Timein)/1000000.0)

// #define Elapsed( _nVal1, _nVal2 )  \
            // (  (_nVal1) > (_nVal2) \
             // ? (_nVal1) - (_nVal2) \
             // : (_nVal2) - (_nVal1) ) 

inline llong Elapsed( llong p_nVal1, llong p_nVal2 ) {
    return (  (p_nVal1) > (p_nVal2) \
            ? (p_nVal1) - (p_nVal2) \
            : (p_nVal2) - (p_nVal1) ) ;
}
// ------------------------------------------------------------------------

Struct( RecordTime_t ) {
    size_t nRanking;           // (Optional use) Numeric quantity, it indicate priority.
    char   szMethod[30];       // Number Method tested.

    size_t array_slots;        // Size of array tested.
    bool   array_TypeSorted    // Type od sort: Asccending / Descending
         , array_IsSorted;     // Flag check: TRUE: is sorted, FALSE: is not sorted.
    int    nCheck_sum;         // checkout sum, for general proposal.

    size_t  anTime[200]        // time list about method evaluation, up to a maximum of 200 evaluations.
         ,  nSelTime           // Time in micro seconds (usec = 10e-6 sec).
         ,  nMinTime           // Minimun time about set of evaluations.
         ,  nMaxTime           // Maximun time about set of evaluations.
         ,  nAvgTime ;         // Average time about set of evaluations.
} ;

int Compare_RecordTime_AvgTime( const void* pRec1, const void* pRec2 ) {
    return Compare( RecordTime_t, *, pRec1, pRec2, nAvgTime );
    // return Compare( PtrVal(RecordTime_t*, pRec1)->nAvgTime, 
                    // PtrVal(RecordTime_t*, pRec2)->nAvgTime ) ;
}
int Compare_RecordTime_MinTime( const void* pRec1, const void* pRec2 ) {
    return Compare( RecordTime_t, *, pRec1, pRec2, nMinTime );
}
int Compare_RecordTime_MaxTime( const void* pRec1, const void* pRec2 ) {
    return Compare( RecordTime_t, *, pRec1, pRec2, nMaxTime );
}
int Compare_RecordTime_SelTime( const void* pRec1, const void* pRec2 ) {
    return Compare( RecordTime_t, *, pRec1, pRec2, nSelTime );
}

// ------------------------------------------------------------------------

inline llong Adjust_Avg( size_t p_nMin, size_t p_nMax, size_t p_nAvg ) {
   llong nDiff_Avg_Min = p_nAvg - p_nMin,
         nDiff_Max_Avg = p_nMax - p_nAvg,
         nFactor       = nDiff_Avg_Min ? nDiff_Max_Avg / nDiff_Avg_Min : 1;
         
   return nDiff_Avg_Min < 2
                           ? p_nAvg
                           : ( nFactor < 2
                               ? p_nAvg
                               : p_nMin +  ( nDiff_Avg_Min < nFactor
                                           ? nFactor / nDiff_Avg_Min
                                           : nDiff_Avg_Min / nFactor )
                             ) ;
}

// ------------------------------------------------------------------------

#define Statistics_From_RecordTime(   _PtrRecordTime, _nMaxEvaluations, ...) csname2(Statistics_From_RecordTime_, Count_Args(__VA_ARGS__))( _PtrRecordTime, _nMaxEvaluations, ##__VA_ARGS__)
#define Statistics_From_RecordTime_0( _PtrRecordTime, _nMaxEvaluations ) Statistics_From_RecordTime_1( (_PtrRecordTime), (_nMaxEvaluations), false )
void Statistics_From_RecordTime_1( RecordTime_t* Record, 
                                   int           p_nEvaluations,
                                   bool          p_bAdjustTime ) {
    ullong nSum = 0 ,
           nMin = (ullong)(( (ullong)0-1)/2-1),
           nMax = 0 ,
           nTime ;
    int nStarIn = p_bAdjustTime 
                    ? (p_nEvaluations<2 ? 0 : 1) 
                    : 0 ;
    for( int i = nStarIn ; i < p_nEvaluations; ++i) {
        nTime = Record->anTime[i] ;
        nSum += nTime ;
        nMin  = Min2( nMin, nTime ) ;
        nMax  = Max2( nMax, nTime ) ;
    }
    Record->nAvgTime = nSum / (p_nEvaluations - nStarIn) ;
    Record->nMinTime = nMin ;
    Record->nMaxTime = nMax ;
    Record->nSelTime = p_bAdjustTime 
                      ? Adjust_Avg( nMin, nMax, Record->nAvgTime ) 
                      : Record->nAvgTime ;
}

void Rankig_Record_Array( RecordTime_t*  *p_pArray, size_t p_nSlots, fnCompare_t fnCompare) {
    QSort_fn( p_pArray, p_nSlots, fnCompare );
    for(int i=0 ; i < p_nSlots ; ++i)
        p_pArray[i]->nRanking = i;
}

// ------------------------------------------------------------------------
// void SortPriority()
// ------------------------------------------------------------------------

// Get timestamp in decimal of nanoseconds
#define  Timestamp_nsec( out_strTime, _a, ...) \
            csname2(Timestamp_nsec_, Count_Args(__VA_ARGS__)) (out_strTime, _a, ##__VA_ARGS__)
#define  Timestamp_nsec_1( _strTime, _nBeginTime, _nEndTime )  Timestamp_nsec_0( _strTime, Elapsed((_nBeginTime), (_nEndTime)) )
char*    Timestamp_nsec_0(char* strTime, llong p_nElapsedTime ) {
    llong nsec, 
          yy, dd,
          hh, mm, ss,
          ms, us, ns;
    if( p_nElapsedTime < 0LL ) p_nElapsedTime *= -1LL ;

               nsec = p_nElapsedTime % 1000000000ll;
     p_nElapsedTime = (p_nElapsedTime - nsec) / 1000000000ll ;  // Here time is in seconds
                 yy = p_nElapsedTime / 31536000 ;               // Secons per year
    p_nElapsedTime -= yy * 31536000 ;           
                 dd = p_nElapsedTime / 86400 ;                  // Secons per day
    p_nElapsedTime -= dd * 86400 ;          
                 hh = p_nElapsedTime / 3600 ;                   // Secons per hour
    p_nElapsedTime -= hh * 3600 ;           
                 mm = p_nElapsedTime / 60 ;                     // Seconds per minute
    p_nElapsedTime -= mm * 60 ;         
                 ss = p_nElapsedTime  ;                         // Seconds
                
    ms = nsec / 1000000ll ; 
    ns = nsec % 1000000ll ;     
    us = ns / 1000ll;     
    ns %= 1000ll;         
    
    int   bytes = 0;
    char* szPos = strTime ;
    if( yy )   { szPos += bytes; bytes = sprintf(szPos, " %d years", yy) ; }
    if( dd )   { szPos += bytes; bytes = sprintf(szPos, " %d days", dd) ; }
    if( hh )   { szPos += bytes; bytes = sprintf(szPos, " %d h", hh) ; }
    if( mm )   { szPos += bytes; bytes = sprintf(szPos, " %d m", mm) ; }
    if( ss )   { szPos += bytes; bytes = sprintf(szPos, " %d s", ss) ; }
    
    // if( ms )   { szPos += bytes; bytes = sprintf(szPos, " %d ms", ms) ; }
    // if( us )   { szPos += bytes; bytes = sprintf(szPos, " %d us", us) ; }
    // if( ns )   { szPos += bytes; bytes = sprintf(szPos, " %d ns", ns) ; }

    if( nsec ) { szPos += bytes; bytes = sprintf(szPos, " %3.6f ms", nsec2msec(nsec)) ; }
    return strTime;
}

#define Timestamp_usec( out_strTime, _a, ...)     csname2(Timestamp_usec_, Count_Args(__VA_ARGS__))(out_strTime, _a, ##__VA_ARGS__)
#define Timestamp_usec_1( _strTime, _nBeginTime, _nEndTime )  Timestamp_usec_0( _strTime, Elapsed((_nEndTime), (_nBeginTime)) )
char*   Timestamp_usec_0(char* strTime, llong p_nElapsedTime ) {
   llong usec,
         yy, dd,
         hh, mm, ss,
         ms, us;
   if( p_nElapsedTime < 0LL ) p_nElapsedTime *= -1LL ;
   
              usec = p_nElapsedTime % 1000000ll;
     p_nElapsedTime = (p_nElapsedTime - usec) / 1000000ll ;  // Here time is in seconds
                 yy = p_nElapsedTime / 31536000 ;            // Secons per year
    p_nElapsedTime -= yy * 31536000 ;                        
                 dd = p_nElapsedTime / 86400 ;               // Secons per day
    p_nElapsedTime -= dd * 86400 ;                           
                 hh = p_nElapsedTime / 3600 ;                // Secons per hour
    p_nElapsedTime -= hh * 3600 ;                            
                 mm = p_nElapsedTime / 60 ;                  // Seconds per minute
    p_nElapsedTime -= mm * 60 ;                              
                 ss = p_nElapsedTime  ;                      // Seconds
    
    ms = usec / 1000ll ; 
    us = usec % 1000ll ;     

   int   bytes = 0;
   char* szPos = strTime ;
   if( yy )   { szPos += bytes; bytes = sprintf(szPos, " %d years", yy) ; }
   if( dd )   { szPos += bytes; bytes = sprintf(szPos, " %d days", dd) ; }
   if( hh )   { szPos += bytes; bytes = sprintf(szPos, " %d h", hh) ; }
   if( mm )   { szPos += bytes; bytes = sprintf(szPos, " %d m", mm) ; }
   if( ss )   { szPos += bytes; bytes = sprintf(szPos, " %d s", ss) ; }

   // if( ms )   { szPos += bytes; bytes = sprintf(szPos, " %d ms", ms) ; }
   // if( us )   { szPos += bytes; bytes = sprintf(szPos, " %d us", us) ; }

    if( usec ) { szPos += bytes; bytes = sprintf(szPos, " %3.3f ms", usec2msec(usec)) ; }
   return strTime;
}

// .............................................................

#ifdef _WINDOWS_ 

static void unix_time(struct timespec *spec) {
   llong wintime; 
   GetSystemTimeAsFileTime((FILETIME*)&wintime); 
   wintime -= 116444736000000000ll;  
   spec->tv_sec  =wintime / 10000000ll;                 
   spec->tv_nsec =wintime % 10000000ll *100;
}

// Auxiliar function from Microsoft Documentation
static int clock_gettime(int dummy, timespec *spec) {
   timespec startspec; 
   double  ticks2nano;
   llong startticks, 
         tps =0;
   llong tmp, curticks;
   QueryPerformanceFrequency(( LARGE_INTEGER*)&tmp ); //some strange system can
   if (tps !=tmp) { 
        tps =tmp; //init ~~ONCE         //possibly change freq ?
        QueryPerformanceCounter( (LARGE_INTEGER*)&startticks );
        unix_time( &startspec ); 
        ticks2nano =(double)1000000000ll / tps; 
   }
   QueryPerformanceCounter( (LARGE_INTEGER*)&curticks); 
   curticks -=startticks;
   spec->tv_sec  =startspec.tv_sec   +         (curticks / tps);
   spec->tv_nsec =startspec.tv_nsec  + (double)(curticks % tps) * ticks2nano;
   if ( !(spec->tv_nsec < 1000000000ll) ) { 
      spec->tv_sec++; 
      spec->tv_nsec -= 1000000000ll; 
   }
   return 0;
}
#endif
 // .............................................................

// Get time in micro seconds (10e-6).
inline llong gettime_usec() {
   llong microsec = -1;
   #if defined(_WIN32) || defined(_WIN64)
      FILETIME  tInfo;
      GetSystemTimeAsFileTime( &tInfo );
      microsec = tInfo.dwHighDateTime;
      microsec <<=32;
      microsec |= tInfo.dwLowDateTime;
      microsec /=10;
      microsec -= 11644473600000000ULL;
   #else
      timeval  tInfo;
      if( !gettimeofday( &tInfo, NULL ) )
         microsec = (llong) tInfo.tv_sec * 1000000ll +
                    (llong) tInfo.tv_usec ;
   #endif
   return microsec ;
}

// (Deprecated) Get time in decimals of nano seconds (10e-7).
inline llong gettime_nsec2() {
   llong nanosec = -1;
   #if defined(_WIN32) || defined(_WIN64)
      FILETIME  tInfo;
      GetSystemTimeAsFileTime( &tInfo );
      nanosec = tInfo.dwHighDateTime;
      nanosec <<=32;
      nanosec |= tInfo.dwLowDateTime;
      nanosec -= 1164447360000000ULL;
   #else
      timeval  tInfo;
      if( !gettimeofday( &tInfo, NULL ) )
         nanosec = ( (llong) tInfo.tv_sec * 1000000ll +
                      (llong) tInfo.tv_usec ) * 10ll ;
   #endif
   return nanosec ;
}

// Get time in decimals of nano seconds (10e-7).
inline llong gettime_nsec() {
   llong nanosec = 0;
   #if defined(_WINDOWS_) || ( (defined(_WIN32) || defined(_WIN64) ) && !defined(_OSX_) )
      timespec  tInfo;
      if( !clock_gettime(CLOCK_MONOTONIC, &tInfo) )
         nanosec = (llong) tInfo.tv_sec * 1000000000ll + 
                   (llong) tInfo.tv_nsec ;
                  
   #elif defined(unix) || defined(__unix) || defined(__unix__) || \
         defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
         defined(__APPLE__) || defined(__MACH__) 
         defined(__ANDROID__) || defined(__linux__) || defined(_OSX_)
      mach_timespec_t tInfo;
      clock_serv_t    cclock;
      host_get_clock_service( mach_host_self(), CALENDAR_CLOCK, &cclock );
      if( clock_get_time(cclock, &tInfo) )
         nanosec = (llong) tInfo.tv_sec * 1000000000ll + 
                   (llong) tInfo.tv_nsec ;
      mach_port_deallocate( mach_task_self(), cclock );
   #endif
   return nanosec ;
}
// ------------------------------------------------------

#ifdef __cplusplus
    }
#endif

#endif // end #define __ebm_time_h__
