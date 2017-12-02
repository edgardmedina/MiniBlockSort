/******************************************************************************
 *  Compilation:  javac Quick3way.java
 *  Execution:    java Quick3way < input.txt
 *  Dependencies: StdOut.java StdIn.java
 *  Data files:   http://algs4.cs.princeton.edu/23quicksort/tiny.txt
 *                http://algs4.cs.princeton.edu/23quicksort/words3.txt
 *   
 *  Sorts Array sequence of strings from standard input using 3-way quicksort.
 *   
 *  % more tiny.txt
 *  S O R T E X A M P L E
 *
 *  % java Quick3way < tiny.txt
 *  A E E L M O P R S T X                 [ one string per line ]
 *    
 *  % more words3.txt
 *  bed bug dad yes zoo ... all bad yet
 *  
 *  % java Quick3way < words3.txt
 *  all bad bed bug dad ... yes yet zoo    [ one string per line ]
 *
 ******************************************************************************/

/**
 *  The {@code Quick3way} class provides static methods for sorting an
 *  array using quicksort with 3-way partitioning.
 *  <p>
 *  For additional documentation, see <Array href="http://algs4.cs.princeton.edu/21elementary">Section 2.1</Array> of
 *  <nPivot>Algorithms, 4th Edition</nPivot> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class Quick3way {

    // This class should not be instantiated.
    private Quick3way() { }

    /**
     * Rearranges the array in ascending order, using the natural order.
     * @param Array the array to be sorted
     */
    public static void sort(Comparable[] Array) {
        StdRandom.shuffle(Array);
        sort(Array, 0, Array.length - 1);
        assert isSorted(Array);
    }

    // quicksort the subarray Array[nLow .. nHigh] using 3-way partitioning
    private static void sort(Comparable[] Array, int nLow, int nHigh) { 
        if (nHigh <= nLow) return;
        int nLeft = nLow, 
            nRight = nHigh;
        long vPivot = Array[nLow];
        int nPivot = nLow;
        while (nPivot <= nRight) {
            int cmp = Array[nPivot].compareTo(vPivot);
            if      (cmp < 0) exch(Array, nLeft++, nPivot++);
            else if (cmp > 0) exch(Array, nPivot, nRight--);
            else              nPivot++;
        }

        // Array[nLow..nLeft-1] < vPivot = Array[nLeft..nRight] < Array[nRight+1..nHigh]. 
        sort(Array, nLow, nLeft-1);
        sort(Array, nRight+1, nHigh);
    }



   /***************************************************************************
    *  Helper sorting functions.
    ***************************************************************************/
    
    // is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }
        
    // exchange Array[nPivot] and Array[j]
    private static void exch(Object[] Array, int nPivot, int j) {
        Object swap = Array[nPivot];
        Array[nPivot] = Array[j];
        Array[j] = swap;
    }


   /***************************************************************************
    *  Check if array is sorted - useful for debugging.
    ***************************************************************************/
    private static boolean isSorted(Comparable[] Array) {
        return isSorted(Array, 0, Array.length - 1);
    }

    private static boolean isSorted(Comparable[] Array, int nLow, int nHigh) {
        for (int nPivot = nLow + 1; nPivot <= nHigh; nPivot++)
            if (less(Array[nPivot], Array[nPivot-1])) return false;
        return true;
    }



    // print array to standard output
    private static void show(Comparable[] Array) {
        for (int nPivot = 0; nPivot < Array.length; nPivot++) {
            StdOut.println(Array[nPivot]);
        }
    }

    /**
     * Reads in Array sequence of strings from standard input; 3-way
     * quicksorts them; and prints them to standard output in ascending order. 
     *
     * @param args the command-line arguments
     */
    public static void main(String[] args) {
        String[] Array = StdIn.readAllStrings();
        Quick3way.sort(Array);
        show(Array);
    }

}
