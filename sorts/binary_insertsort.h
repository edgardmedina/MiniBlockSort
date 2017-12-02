#ifndef __bynary_insertsort_h__
#define __bynary_insertsort_h__

static __inline size_t  BINARY_INSERTION_FIND(      long *p_pArray, const long x, const size_t size) ;
static void             BINARY_INSERTION_SORT_START(long *p_pArray, const size_t start, const size_t size);
void                    BINARY_INSERTION_SORT(      long *p_pArray, const size_t size) ;

#define BinaryInsertionSort( _Array, nLength, ...)      csname2(BinaryInsertionSort_, Count_Args(__VA_ARGS__))( _Array, nLength, ##__VA_ARGS__)
#define BinaryInsertionSort_0( Array, nLength)                        BINARY_INSERTION_SORT( (Array), (nLength) )
#define BinaryInsertionSort_1( Array, nLength, _bIsAscending )        BINARY_INSERTION_SORT( (Array), (nLength) )
#define BinaryInsertionSort_2( Array, nLength, _Type, _bIsAscending)  BINARY_INSERTION_SORT( (Array), (nLength) )


#ifndef SORT_CMP
    #define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
#endif

#ifndef SORT_SWAP
    #define SORT_SWAP(TYPE, x,y) {TYPE _TDAta = (x); (x) = (y); (y) = _TDAta;}
#endif


/* Binary insertion sort */
void BINARY_INSERTION_SORT(long *p_pArray, const size_t size) {
  /* don't bother sorting an array of size <= 1 */
  if (size < 2) 
    return;

  BINARY_INSERTION_SORT_START(p_pArray, 1, size);
}
// ....................................................................
#ifndef CLZ
    #ifdef __GNUC__
        #define CLZ __builtin_clzll
        
    #else
        
        #define CLZ clzll

        static int clzll(uint64_t);

        /* adapted from Hacker's Delight */
        static int clzll(uint64_t x) {
          int n;

          if (x == 0) {
            return 64;
          }

          n = 0;

          if (x <= 0x00000000FFFFFFFFL) {
            n = n + 32;
            x = x << 32;
          }

          if (x <= 0x0000FFFFFFFFFFFFL) {
            n = n + 16;
            x = x << 16;
          }

          if (x <= 0x00FFFFFFFFFFFFFFL) {
            n = n + 8;
            x = x << 8;
          }

          if (x <= 0x0FFFFFFFFFFFFFFFL) {
            n = n + 4;
            x = x << 4;
          }

          if (x <= 0x3FFFFFFFFFFFFFFFL) {
            n = n + 2;
            x = x << 2;
          }

          if (x <= 0x7FFFFFFFFFFFFFFFL) {
            n = n + 1;
          }

          return n;
        }

    #endif
#endif


// ....................................................................
/*
// This functions is obtained from:  https://embeddedgurus.com/state-space/tag/clz/
#define CLZ   CLZ2
static inline uint32_t CLZ2(uint32_t x) {
    static uint8_t const clz_lkup[] = {
        32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
        28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U
    };
    uint32_t n;

    if (x >= (1U << 16)) {
        if (x >= (1U << 24)) {
            if (x >= (1 << 28)) {
                n = 28U;
            }
            else {
                n = 24U;
            }
        }
        else {
            if (x >= (1U << 20)) {
                n = 20U;
            }
            else {
                n = 16U;
            }
        }
    }
    else {
        if (x >= (1U << 8)) {
            if (x >= (1U << 12)) {
                n = 12U;
            }
            else {
                n = 8U;
            }
        }
        else {
            if (x >= (1U << 4)) {
                n = 4U;
            }
            else {
                n = 0U;
            }
        }
    }
    return (uint32_t)clz_lkup[x >> n] - n;
}
/**/
// ......................................................................

/* Binary insertion sort, but knowing that the first "start" entries are sorted.  Used in timsort. */
static void BINARY_INSERTION_SORT_START(long *p_pArray, const size_t start, const size_t size) {
    size_t i;
    size_t j;
    long x;
    size_t location;

  for (i = start; i < size; i++) {

    /* If this entry is already correct, just move along */
    if (SORT_CMP(p_pArray[i - 1], p_pArray[i]) <= 0) {
      continue;
    }

    /* Else we need to find the right place, shift everything over, and squeeze in */
    x = p_pArray[i];
    location = BINARY_INSERTION_FIND(p_pArray, x, i);

    for (j = i - 1; j >= location; j--) {
      p_pArray[j + 1] = p_pArray[j];

      if (j == 0) { /* check edge case because j is unsigned */
        break;
      }
    }

    p_pArray[location] = x;
  }
}

// ......................................................................

/* Function used to do a binary search for binary insertion sort */
static __inline size_t BINARY_INSERTION_FIND(long *p_pArray, const long x, const size_t size) {
  size_t l, c, r;
  long cx;
  l = 0;
  r = size - 1;
  c = r >> 1;

  /* check for out of bounds at the beginning. */
  if (SORT_CMP(x, p_pArray[0]) < 0) {
    return 0;
  } else if (SORT_CMP(x, p_pArray[r]) > 0) {
    return r;
  }

  cx = p_pArray[c];

  while (1) {
    const int val = SORT_CMP(x, cx);

    if (val < 0) {
      if (c - l <= 1) {
        return c;
      }

      r = c;
    } else { /* allow = for stability. The binary search favors the right. */
      if (r - c <= 1) {
        return c + 1;
      }

      l = c;
    }

    c = l + ((r - l) >> 1);
    cx = p_pArray[c];
  }
}

#endif  // #define __bynary_insertsort_h__
