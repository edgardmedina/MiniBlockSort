
/******************************************************************************
 *  Compilation:  javac LSD.java
 *  Execution:    java LSD < input.txt
 *  Dependencies: StdIn.java StdOut.java
 *  Data files:   http://algs4.cs.princeton.edu/51radix/words3.txt
 *
 *  LSD radix sort
 *
 *    - Sort a String[] array of n extended ASCII strings (BYTE_RANGE = 256), each of length nWide.
 *
 *    - Sort an int[] array of n 32-bit integers, treating each integer as
 *      a sequence of nWide = 4 bytes (BYTE_RANGE = 256).
 *
 *  Uses extra space proportional to n + BYTE_RANGE.
 *
 *
 *  % java LSD < words3.txt
 *  all
 *  bad
 *  bed
 *  bug
 *  dad
 *  ...
 *  yes
 *  yet
 *  zoo
 *
 ******************************************************************************/

/**
 *  The {@code LSD} class provides static methods for sorting an
 *  array of nWide-character strings or 32-bit integers using LSD radix sort.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/51radix">Section 5.1</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

/**
  * Rearranges the array of 32-bit integers in ascending order.
  * This is about 2-3x faster than Arrays.sort().
  *
  * @param a the array to be sorted
  **/
  
enum _CONST_BYTES_{ 
    BYTE_BITS = 8
   ,BYTE_RANGE = 1 << BYTE_BITS
   ,BYTE_MAX_VAL = BYTE_RANGE - 1
} ;
#define DIGIT( _val, _nRadix)   ( ((_val) >> ((_nRadix) * BYTE_BITS)) & BYTE_MAX_VAL)

#define RadixSortLSD( _Array, _nLength, ...) csname2(RadixSortLSD_, Count_Args(__VA_ARGS__))( (_Array), (_nLength), ##__VA_ARGS__)
#define RadixSortLSD_0( Array, _nLength)                        RadixSortLSD_Int( Array, _nLength  )
#define RadixSortLSD_1( Array, _nLength, _bIsAscending)         RadixSortLSD_Int( Array, _nLength )
#define RadixSortLSD_2( Array, _nLength, _Type, _bIsAscending)  RadixSortLSD_Int( Array, _nLength )

void RadixSortLSD_Int(long* Array, size_t nSlotsArray) {
   int nBytes = sizeof(*Array);  // each int is 4 bytes
   long* arrAux = New(int, nSlotsArray);

   // compute frequency counts
   int* anCount = New( int, BYTE_RANGE+1);
   
   for (int nRadix = 0; nRadix < nBytes; nRadix++) {
      
      memset( anCount, 0, sizeof(int)*(BYTE_RANGE+1) ); // compute frequency counts
      for (int i = 0; i < nSlotsArray; ++i) 
          ++anCount[ 1 + DIGIT(Array[i], nRadix) ];

      for (int r = 0; r < BYTE_RANGE; ++r) // compute cumulates
          anCount[r+1] += anCount[r];

      if (nRadix == nBytes-1) { // for most significant byte, 0x80-0xFF comes before 0x00-0x7F
         int shift1 = anCount[BYTE_RANGE] - anCount[BYTE_RANGE/2];
         int shift2 = anCount[BYTE_RANGE/2];
         for (int r = 0; r < BYTE_RANGE/2; r++)
            anCount[r] += shift1;
         for (int r = BYTE_RANGE/2; r < BYTE_RANGE; r++)
            anCount[r] -= shift2;
      }

      for (int i = 0; i < nSlotsArray; ++i)   // move data
         arrAux[ anCount[ DIGIT(Array[i], nRadix) ]++ ] = Array[i];
      
      for (int i = 0; i < nSlotsArray; i++) // copy back
         Array[i] = arrAux[i];
   }
   Delete(arrAux);
   Delete(anCount);
}

/**
  * Rearranges the array of W-character strings in ascending order.
  *
  * @param a the array to be sorted
  * @param nStrWide the number of characters per string
  **/
void RadixSortLSD_Str(char* arrStr[], size_t nSlotsArray, int nStrWide) {
   int RSTRVALUES = 256;   // extend ASCII alphabet size
   char** arrAux = New( char*, nSlotsArray);
   
   // compute frequency counts
   int* anCount = New(int, RSTRVALUES+1);

   // sort by key-indexed counting on dth character
   for (int nRadix = nStrWide-1; nRadix >= 0; nRadix--) {

      // compute frequency counts
      memset(anCount, 0, sizeof(int)*(RSTRVALUES+1) );
      
      for (int i = 0; i < nSlotsArray; i++)
         anCount[arrStr[i][nRadix] + 1]++;

      // compute cumulates
      for (int r = 0; r < RSTRVALUES; r++)
          anCount[r+1] += anCount[r];

      // move data
      for (int i = 0; i < nSlotsArray; i++)
         arrAux[ anCount[arrStr[i][nRadix]]++ ] = arrStr[i];

      // copy back
      for (int i = 0; i < nSlotsArray; i++)
          arrStr[i] = arrAux[i];
   }
   Delete(anCount);
   Delete(arrAux);
}

/**
 * Reads in a sequence of fixed-length strings from standard input;
 * LSD radix sorts them;
 * and prints them to standard output in ascending order.
 *
 * @param args the command-line arguments
 **/
// void _main(String[] args) {
   // String[] a = StdIn.readAllStrings();
   // int n = a.length;

   // // check that strings have fixed length
   // int nWide = a[0].length();
   // for (int i = 0; i < n; i++)
      // assert a[i].length() == nWide : "Strings must have fixed length";

   // // sort the strings
   // sort(a, nWide);

   // // print results
   // for (int i = 0; i < n; i++)
      // StdOut.println(a[i]);
// }

