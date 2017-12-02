#ifndef __mergesort_inplace1_h__
#define __mergesort_inplace1_h__

#include "binary_insertsort.h"

static __inline size_t rbnd(size_t len) ;
void   MERGE_SORT_IN_PLACE_ASWAP(long * pArray, long * dst2, size_t len) ;
void   MERGE_SORT_IN_PLACE_FRONTMERGE(long *pArray, size_t l1, long *dst2, size_t l2) ;
size_t MERGE_SORT_IN_PLACE_BACKMERGE(long * pArray, size_t l1, long * dst2, size_t l2) ;
void   MERGE_SORT_IN_PLACE_RMERGE(long *p_pArray, size_t len, size_t lp, size_t r) ;

// .....................................................................


#define Mergesort_InPlace( _Array, ...)     csname2(Mergesort_InPlace_, Count_Args(__VA_ARGS__))( (_Array), ##__VA_ARGS__)
#define Mergesort_InPlace_0( _Array )                                MERGE_SORT_IN_PLACE( _Array, sizeofarray(_Array), ASC)
#define Mergesort_InPlace_1( _Array, _nSlots)                        MERGE_SORT_IN_PLACE( _Array, (_nSlots) )
#define Mergesort_InPlace_2( _Array, _nSlots, _bIsAscending )        MERGE_SORT_IN_PLACE( _Array, (_nSlots) )
#define Mergesort_InPlace_3( _Array, _nSlots, _Type, _bIsAscending)  MERGE_SORT_IN_PLACE( _Array, (_nSlots) )

/* In-place Merge Sort implementation. (c)2012, Andrey Astrelin, astrelin@tochka.ru */
void MERGE_SORT_IN_PLACE(long *p_pArray, const size_t len) {
  /* don't bother sorting an array of size <= 1 */
  if (len <= 1) {
    return;
  }

  size_t r  = rbnd(len);
  size_t lr = (len / r - 1) * r, 
          p, m, q, q1, p0;
  long *pArray = p_pArray - 1;

  if (len < 16) {
    BINARY_INSERTION_SORT(p_pArray, len);
    return;
  }

  for (p = 2; p <= lr; p += 2) {
    pArray += 2;

    if (SORT_CMP(pArray[0], pArray[-1]) < 0) {
      SORT_SWAP(long, pArray[0], pArray[-1]);
    }

    if (p & 2) {
      continue;
    }

    m = len - p;
    q = 2;

    while ((p & q) == 0) {
      if (SORT_CMP(pArray[1 - q], pArray[-q]) < 0) {
        break;
      }

      q *= 2;
    }

    if (p & q) {
      continue;
    }

    if (q < m) {
      p0 = len - q;
      MERGE_SORT_IN_PLACE_ASWAP(p_pArray + p - q, p_pArray + p0, q);

      for (;;) {
        q1 = 2 * q;

        if ((q1 > m) || (p & q1)) {
          break;
        }

        p0 = len - q1;
        MERGE_SORT_IN_PLACE_FRONTMERGE(p_pArray + (p - q1), q, p_pArray + p0 + q, q);
        q = q1;
      }

      MERGE_SORT_IN_PLACE_BACKMERGE(p_pArray + (len - 1), q, pArray - q, q);
      q *= 2;
    }

    q1 = q;

    while (q1 > m) {
      q1 /= 2;
    }

    while ((q & p) == 0) {
      q *= 2;
      MERGE_SORT_IN_PLACE_RMERGE(p_pArray + (p - q), q, q / 2, q1);
    }
  }

  q1 = 0;

  for (q = r; q < lr; q *= 2) {
    if ((lr & q) != 0) {
      q1 += q;

      if (q1 != q) {
        MERGE_SORT_IN_PLACE_RMERGE(p_pArray + (lr - q1), q1, q, r);
      }
    }
  }

  m = len - lr;
  MERGE_SORT_IN_PLACE(p_pArray + lr, m);
  MERGE_SORT_IN_PLACE_ASWAP(p_pArray, p_pArray + lr, m);
  m += MERGE_SORT_IN_PLACE_BACKMERGE(p_pArray + (m - 1), m, p_pArray + (lr - 1), lr - m);
  MERGE_SORT_IN_PLACE(p_pArray, m);
}

// .....................................................................

static __inline size_t rbnd(size_t len) {
  int k;

  if (len < 16) {
    return 2;
  }

  k = 62 - CLZ(len);
  return 1ULL << ((2 * k) / 3);
}

/* In-place mergesort */
void MERGE_SORT_IN_PLACE_ASWAP(long * pArray, long * dst2, size_t len) {
  long tmp;
  do {
    SORT_SWAP(long, *pArray, *dst2);
    pArray++;
    dst2++;
  } while (--len);
}

void MERGE_SORT_IN_PLACE_FRONTMERGE(long *pArray, size_t l1, long *dst2, size_t l2) {
  long *dst0 = dst2 - l1;
  long tmp;
  if (SORT_CMP(pArray[l1 - 1], dst2[0]) <= 0) {
    MERGE_SORT_IN_PLACE_ASWAP(pArray, dst0, l1);
    return;
  }

  do {
    while (SORT_CMP(*dst2, *pArray) > 0) {
      SORT_SWAP(long, *pArray, *dst0);
      pArray++;
      dst0++;

      if (--l1 == 0) {
        return;
      }
    }

    SORT_SWAP(long, *dst2, *dst0 );
    dst2++;
    dst0++;
  } while (--l2);

  do {
    SORT_SWAP(long, *pArray, *dst0 );
    pArray++;
    dst0++;
  } while (--l1);
}

size_t MERGE_SORT_IN_PLACE_BACKMERGE(long * pArray, size_t l1, long * dst2, size_t l2) {
  size_t res;
  long *dst0 = dst2 + l1;
  long tmp;
  if (SORT_CMP(pArray[1 - l1], dst2[0]) >= 0) {
    MERGE_SORT_IN_PLACE_ASWAP(pArray - l1 + 1, dst0 - l1 + 1, l1);
    return l1;
  }

  do {
    while (SORT_CMP(*dst2, *pArray) < 0) {
      SORT_SWAP(long, *pArray, *dst0);
      pArray--;
      dst0--;

      if (--l1 == 0) {
        return 0;
      }
    }

    SORT_SWAP(long, *dst2, *dst0);
    dst2--;
    dst0--;
  } while (--l2);

  res = l1;

  do {
    SORT_SWAP(long, *pArray, *dst0);
    pArray--;
    dst0--;
  } while (--l1);

  return res;
}

// ..............................................................
/* merge p_pArray[p0..p1) by buffer p_pArray[p1..p1+r) */
void MERGE_SORT_IN_PLACE_RMERGE(long *p_pArray, size_t len, size_t lp, size_t r) {
  size_t i, lq;
  int cv;

  if (SORT_CMP(p_pArray[lp], p_pArray[lp - 1]) >= 0) {
    return;
  }

  lq = lp;

  for (i = 0; i < len; i += r) {
    /* select smallest p_pArray[p0+n*r] */
    size_t q = i, j;

    for (j = lp; j <= lq; j += r) {
      cv = SORT_CMP(p_pArray[j], p_pArray[q]);

      if (cv == 0) {
        cv = SORT_CMP(p_pArray[j + r - 1], p_pArray[q + r - 1]);
      }

      if (cv < 0) {
        q = j;
      }
    }

    if (q != i) {
      MERGE_SORT_IN_PLACE_ASWAP(p_pArray + i, p_pArray + q, r); /* swap it with current position */

      if (q == lq && q < (len - r)) {
        lq += r;
      }
    }

    if (i != 0 && SORT_CMP(p_pArray[i], p_pArray[i - 1]) < 0) {
      MERGE_SORT_IN_PLACE_ASWAP(p_pArray + len, p_pArray + i, r); /* swap current position with buffer */
      MERGE_SORT_IN_PLACE_BACKMERGE(p_pArray + (len + r - 1), r, p_pArray + (i - 1),
                                    r);  /* buffer :merge: p_pArray[i-r..i) -> p_pArray[i-r..i+r) */
    }

    if (lp == i) {
      lp += r;
    }
  }
}

#endif // #ifndef __mergesort_inplace1_h__

