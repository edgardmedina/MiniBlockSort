#ifndef  __timsort_h__
#define  __timsort_h__

#include "binary_insertsort.h"

typedef struct {
  size_t alloc;
  long *storage;
} Temp_Storage_t;

typedef struct {
  size_t start;
  size_t length;
} Tim_Sort_Run_t;

enum{
    TIM_SORT_STACK_SIZE=128
};

static int compute_minrun(const uint64_t);

static __inline void REVERSE_ELEMENTS(long *p_pArray, size_t start, size_t end) ;
static size_t               COUNT_RUN(long *p_pArray, const size_t start, const size_t size) ;
static int            CHECK_INVARIANT(Tim_Sort_Run_t *stack, const int stack_curr) ;
static void           TIM_SORT_RESIZE(Temp_Storage_t *store, const size_t new_size) ;
static void            TIM_SORT_MERGE(long *p_pArray, const Tim_Sort_Run_t *stack, const int stack_curr,  Temp_Storage_t *store) ;
static int          TIM_SORT_COLLAPSE(long *p_pArray, Tim_Sort_Run_t *stack, int stack_curr,
                             Temp_Storage_t *store, const size_t size) ;
static __inline int PUSH_NEXT(long *p_pArray,
                              const size_t size,
                              Temp_Storage_t *store,
                              const size_t minrun,
                              Tim_Sort_Run_t *run_stack,
                              size_t *stack_curr,
                              size_t *curr) ;
void TIM_SORT(long *p_pArray, const size_t size) ;

#define TimSort( _Array, ...)     csname2(TimSort_, Count_Args(__VA_ARGS__))( (_Array), ##__VA_ARGS__)
#define TimSort_0( _Array )                                TIM_SORT( _Array, sizeofarray(_Array) )
#define TimSort_1( _Array, _nSlots)                        TIM_SORT( _Array, (_nSlots) )
#define TimSort_2( _Array, _nSlots, _bIsAscending )        TIM_SORT( _Array, (_nSlots) )
#define TimSort_3( _Array, _nSlots, _Type, _bIsAscending)  TIM_SORT( _Array, (_nSlots) )

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif


void TIM_SORT(long *p_pArray, const size_t size) {
  size_t minrun;
  Temp_Storage_t _store, *store;
  Tim_Sort_Run_t run_stack[TIM_SORT_STACK_SIZE];
  size_t stack_curr = 0;
  size_t curr = 0;

  /* don't bother sorting an array of size 1 */
  if (size <= 1) {
    return;
  }

  if (size < 64) {
    BINARY_INSERTION_SORT(p_pArray, size);
    return;
  }

  /* compute the minimum run length */
  minrun = compute_minrun(size);
  /* temporary storage for merges */
  store = &_store;
  store->alloc = 0;
  store->storage = NULL;

  if (!PUSH_NEXT(p_pArray, size, store, minrun, run_stack, &stack_curr, &curr)) {
    return;
  }

  if (!PUSH_NEXT(p_pArray, size, store, minrun, run_stack, &stack_curr, &curr)) {
    return;
  }

  if (!PUSH_NEXT(p_pArray, size, store, minrun, run_stack, &stack_curr, &curr)) {
    return;
  }

  while (1) {
    if (!CHECK_INVARIANT(run_stack, stack_curr)) {
      stack_curr = TIM_SORT_COLLAPSE(p_pArray, run_stack, stack_curr, store, size);
      continue;
    }

    if (!PUSH_NEXT(p_pArray, size, store, minrun, run_stack, &stack_curr, &curr)) {
      return;
    }
  }
}

static __inline int compute_minrun(const uint64_t size) {
  const int top_bit = 64 - CLZ(size);
  const int shift   = MAX(top_bit, 6) - 6;
  const int minrun  = size >> shift;
  const uint64_t mask = (1ULL << shift) - 1;

  if (mask & size) {
    return minrun + 1;
  }

  return minrun;
}

/* timsort implementation, based on timsort.txt */

static __inline void REVERSE_ELEMENTS(long *p_pArray, size_t start, size_t end) {
  long tmp;
  while (1) {
    if (start >= end) {
      return;
    }

    Swap(p_pArray[start], p_pArray[end], tmp);
    start++;
    end--;
  }
}

static size_t COUNT_RUN(long *p_pArray, const size_t start, const size_t size) {
  size_t curr;

  if (size - start == 1) {
    return 1;
  }
  if (start >= size - 2) {
    if (SORT_CMP(p_pArray[size - 2], p_pArray[size - 1]) > 0) {
      long tmp;
      Swap(p_pArray[size - 2], p_pArray[size - 1], tmp);
    }

    return 2;
  }

  curr = start + 2;

  if (SORT_CMP(p_pArray[start], p_pArray[start + 1]) <= 0) {
    /* increasing run */
    while (1) {
      if (curr == size - 1) {
        break;
      }

      if (SORT_CMP(p_pArray[curr - 1], p_pArray[curr]) > 0) {
        break;
      }

      curr++;
    }

    return curr - start;
  } else {
    /* decreasing run */
    while (1) {
      if (curr == size - 1) {
        break;
      }

      if (SORT_CMP(p_pArray[curr - 1], p_pArray[curr]) <= 0) {
        break;
      }

      curr++;
    }

    /* reverse in-place */
    REVERSE_ELEMENTS(p_pArray, start, curr - 1);
    return curr - start;
  }
}

static int CHECK_INVARIANT(Tim_Sort_Run_t *stack, const int stack_curr) {
  size_t A, B, C;

  if (stack_curr < 2) {
    return 1;
  }

  if (stack_curr == 2) {
    const size_t A1 = stack[stack_curr - 2].length;
    const size_t B1 = stack[stack_curr - 1].length;

    if (A1 <= B1) {
      return 0;
    }

    return 1;
  }

  A = stack[stack_curr - 3].length;
  B = stack[stack_curr - 2].length;
  C = stack[stack_curr - 1].length;

  if ((A <= B + C) || (B <= C)) {
    return 0;
  }

  return 1;
}


static void TIM_SORT_RESIZE(Temp_Storage_t *store, const size_t new_size) {
  if (store->alloc < new_size) {
    long *tempstore = (long *)realloc(store->storage, new_size * sizeof(long));

    if (tempstore == NULL) {
      fprintf(stderr, "Error allocating temporary storage for tim sort: need %lu bytes",
              (unsigned long)(sizeof(long) * new_size));
      exit(1);
    }

    store->storage = tempstore;
    store->alloc = new_size;
  }
}


static void TIM_SORT_MERGE(long *p_pArray, const Tim_Sort_Run_t *stack, const int stack_curr,
                           Temp_Storage_t *store) {
  const size_t A = stack[stack_curr - 2].length;
  const size_t B = stack[stack_curr - 1].length;
  const size_t curr = stack[stack_curr - 2].start;
  long *storage;
  size_t i, j, k;
  TIM_SORT_RESIZE(store, MIN(A, B));
  storage = store->storage;

  /* left merge */
  if (A < B) {
    memcpy(storage, &p_pArray[curr], A * sizeof(long));
    i = 0;
    j = curr + A;

    for (k = curr; k < curr + A + B; k++) {
      if ((i < A) && (j < curr + A + B)) {
        if (SORT_CMP(storage[i], p_pArray[j]) <= 0) {
          p_pArray[k] = storage[i++];
        } else {
          p_pArray[k] = p_pArray[j++];
        }
      } else if (i < A) {
        p_pArray[k] = storage[i++];
      } else {
        break;
      }
    }
  } else {
    /* right merge */
    memcpy(storage, &p_pArray[curr + A], B * sizeof(long));
    i = B;
    j = curr + A;
    k = curr + A + B;

    while (k-- > curr) {
      if ((i > 0) && (j > curr)) {
        if (SORT_CMP(p_pArray[j - 1], storage[i - 1]) > 0) {
          p_pArray[k] = p_pArray[--j];
        } else {
          p_pArray[k] = storage[--i];
        }
      } else if (i > 0) {
        p_pArray[k] = storage[--i];
      } else {
        break;
      }
    }
  }
}


static int TIM_SORT_COLLAPSE(long *p_pArray, Tim_Sort_Run_t *stack, int stack_curr,
                             Temp_Storage_t *store, const size_t size) {
  while (1) {
    size_t A, B, C, D;
    int ABC, BCD, CD;

    /* if the stack only has one thing on it, we are done with the collapse */
    if (stack_curr <= 1) {
      break;
    }

    /* if this is the last merge, just do it */
    if ((stack_curr == 2) && (stack[0].length + stack[1].length == size)) {
      TIM_SORT_MERGE(p_pArray, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    }
    /* check if the invariant is off for a stack of 2 elements */
    else if ((stack_curr == 2) && (stack[0].length <= stack[1].length)) {
      TIM_SORT_MERGE(p_pArray, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    } else if (stack_curr == 2) {
      break;
    }

    B = stack[stack_curr - 3].length;
    C = stack[stack_curr - 2].length;
    D = stack[stack_curr - 1].length;

    if (stack_curr >= 4) {
      A = stack[stack_curr - 4].length;
      ABC = (A <= B + C);
    } else {
      ABC = 0;
    }

    BCD = (B <= C + D) || ABC;
    CD = (C <= D);

    /* Both invariants are good */
    if (!BCD && !CD) {
      break;
    }

    /* left merge */
    if (BCD && !CD) {
      TIM_SORT_MERGE(p_pArray, stack, stack_curr - 1, store);
      stack[stack_curr - 3].length += stack[stack_curr - 2].length;
      stack[stack_curr - 2] = stack[stack_curr - 1];
      stack_curr--;
    } else {
      /* right merge */
      TIM_SORT_MERGE(p_pArray, stack, stack_curr, store);
      stack[stack_curr - 2].length += stack[stack_curr - 1].length;
      stack_curr--;
    }
  }

  return stack_curr;
}


static __inline int PUSH_NEXT(long *p_pArray,
                              const size_t size,
                              Temp_Storage_t *store,
                              const size_t minrun,
                              Tim_Sort_Run_t *run_stack,
                              size_t *stack_curr,
                              size_t *curr) {
  size_t len = COUNT_RUN(p_pArray, *curr, size);
  size_t run = minrun;

  if (run > size - *curr) {
    run = size - *curr;
  }

  if (run > len) {
    BINARY_INSERTION_SORT_START(&p_pArray[*curr], len, run);
    len = run;
  }

  run_stack[*stack_curr].start = *curr;
  run_stack[*stack_curr].length = len;
  (*stack_curr)++;
  *curr += len;

  if (*curr == size) {
    /* finish up */
    while (*stack_curr > 1) {
      TIM_SORT_MERGE(p_pArray, run_stack, *stack_curr, store);
      run_stack[*stack_curr - 2].length += run_stack[*stack_curr - 1].length;
      (*stack_curr)--;
    }

    if (store->storage != NULL) {
      free(store->storage);
      store->storage = NULL;
    }

    return 0;
  }

  return 1;
}


#endif  // #ifndef __timsort_h__

