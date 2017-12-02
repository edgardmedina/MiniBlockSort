/***************************************************************************************
 * @Algorithm :  Quick Sort.
 *               Simple adaptation to 'C99' or 'C++98' from:
 *                 @Source       : Bynary Insrtion Sort
 *                 @author       : Jeffrey Stedfast
 *                 @version      : 2007.02.25 
 * 
 * @Adapted by: Edgard Medina Q. <edgardmedina@yahoo.com>. Arequipa, PerÃº.
 ***************************************************************************************/
#define Binary_Insertion_Sort( _Array, nLength, ...)      csname2(Binary_Insertion_Sort_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define Binary_Insertion_Sort_0( Array, nLength)                        Binary_Insertion_Sort_Optimized( (Array), (nLength) )
#define Binary_Insertion_Sort_1( Array, nLength, _bIsAscending )        Binary_Insertion_Sort_Optimized( (Array), (nLength) )
#define Binary_Insertion_Sort_2( Array, nLength, _Type, _bIsAscending)  Binary_Insertion_Sort_Optimized( (Array), (nLength) )


void Binary_Insertion_Sort_Optimized (int p_pArray[], int p_nSlots) {
    register int i, nMid;
    int nHight, nLow, datum;
    for (i = 1; i < p_nSlots; i++) {
        nLow = 0, nHight = i;
        nMid = i / 2;
        do {
            if (p_pArray[i] > p_pArray[nMid]) {
                nLow = nMid + 1;
            } else if (p_pArray[i] < p_pArray[nMid]) {
                nHight = nMid;
            } else
                break;
            nMid = nLow + ((nHight - nLow) / 2);
        } while (nLow < nHight);
        if (nMid < i) {
            datum = p_pArray[i];
            memmove (p_pArray + nMid + 1, p_pArray + nMid, sizeof (int) * (i - nMid));
            p_pArray[nMid] = datum;
        }
    }
}