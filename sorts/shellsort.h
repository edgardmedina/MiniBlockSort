#ifndef __shellsort_h__
#define __shellsort_h__

/* From http://oeis.org/classic/A102549 */
static const long ganShell_Gaps[] = {
        1, 4, 10, 23, 57, 132, 301, 701, 1750, 4376, 10941, 27353, 68383, 170958, 
        427396, 1068491, 2671228, 6678071, 16695178, 41737946, 104344866, 260862166, 
        652155416, 1630388541, 4075971353LL, 10189928383LL, 25474820958LL, 
        63687052396LL  };
        
// static const size_t ganShell_Gaps[48] = {
        // 1, 4, 10, 23, 57, 132, 301, 701, 1750, 4376, 10941, 27353, 68383, 170958, 
        // 427396, 1068491, 2671228, 6678071, 16695178, 41737946, 104344866, 260862166, 
        // 652155416, 1630388541, 4075971353LL, 10189928383LL, 25474820958LL, 
        // 63687052396LL, 
        // 159217630991LL, 398044077478LL, 995110193696LL, 2487775484241LL, 6219438710603LL, 
        // 15548596776508LL, 38871491941271LL, 97178729853178LL, 242946824632946LL, 607367061582366LL, 
        // 1518417653955916LL, 3796044134889791LL, 9490110337224478LL, 23725275843061196LL, 
        // 59313189607652991LL, 148282974019132478LL, 370707435047831196LL, 926768587619577991LL, 
        // 2316921469048944978LL, 5792303672622362446LL };


#define ShellSort( _Array, nLength, ...) csname2(ShellSort_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define ShellSort_0( Array, nLength)                        _SHELL_SORT( (Array), (nLength) )
#define ShellSort_1( Array, nLength, _bIsAscending )        _SHELL_SORT( (Array), (nLength) )
#define ShellSort_2( Array, nLength, _Type, _bIsAscending)  _SHELL_SORT( (Array), (nLength) )
        
/* Shell sort implementation based on Wikipedia article
   http://en.wikipedia.org/wiki/Shell_sort
*/
void _SHELL_SORT(long pArray[], const size_t nLengh) {
    /* don't bother sorting an array of nLengh 0 or 1 */
    if (nLengh <= 1) 
        return;

    //------------------------------------------
    // TODO: binary search to find first gap? 
    int inci = 47;
    size_t inc = ganShell_Gaps[inci];
    size_t i;

    // inc = Search_MinOfMax(nLengh, ganShell_Gaps, 0, sizeofarray(ganShell_Gaps)-1 );
    
    while (inc > (nLengh >> 1)) 
        inc = ganShell_Gaps[--inci];
    //------------------------------------------

    while( true ) {
        for (i = inc; i < nLengh; i++) {
            long temp = pArray[i];
            size_t j = i;

            while( j >= inc && Compare( temp, pArray[j - inc] ) < 0 ) {
                pArray[j] = pArray[j - inc];
                j -= inc;
            }

            pArray[j] = temp;
        }
        if (inc == 1)   
            break;
        inc = ganShell_Gaps[--inci];
    }
}

static void SellSort2(long a[], const size_t nLengh ) { // Sort a[] into increasing order.
    int N = nLengh ;
    int h = 1;
    while( h < N/3 ) 
        h = 3*h + 1; // 1, 4, 13, 40, 121, 364, 1093, ...
    
    long tmp;
    while (h >= 1)    { // h-sort the array.
        for (int i = h; i < N; i++) { // Insert a[i] among a[i-h], a[i-2*h], a[i-3*h]... .
            for (int j = i; j >= h && Compare(a[j], a[j-h]) < 0 ; j -= h)
                Swap(a[j], a[j-h], tmp);
        }
        h = h/3;
    }
}

#endif // #define __shellsort_h__
