#include "simd.h"
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

long long int sum(int vals[NUM_ELEMS]) {
  clock_t start = clock();

  long long int sum = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  long long int sum = 0;

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      if (vals[i] >= 128)
        sum += vals[i];
      if (vals[i + 1] >= 128)
        sum += vals[i + 1];
      if (vals[i + 2] >= 128)
        sum += vals[i + 2];
      if (vals[i + 3] >= 128)
        sum += vals[i + 3];
    }

    // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
    // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
    // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(
      127); // This is a vector with 127s in it... Why might you need this?
  long long int result = 0; // This is where you should put your final result!
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* YOUR CODE GOES HERE */
    int yu = NUM_ELEMS % 4;
    for (int i = 0; i < yu; ++i) {
      if (vals[i] >= 128)
        result += vals[i];
    }
    for (int i = yu; i < NUM_ELEMS; i += 4) {
      __m128i cmp = _mm_setzero_si128();
      __m128i out = _mm_setzero_si128();
      __m128i tmp = _mm_loadu_si128((__m128i *)(vals + i));
      cmp = _mm_cmpgt_epi32(tmp, _127);
      out = _mm_and_si128(cmp, tmp);
      int tmp_arr[4] = {0};
      _mm_storeu_si128((__m128i *)tmp_arr, out);
      result += (tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3]);
    }
    /* Hint: you'll need a tail case. */
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(127);
  long long int result = 0;
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* YOUR CODE GOES HERE */
    /* Copy your sum_simd() implementation here, and unroll it */
    int yu = NUM_ELEMS % 16;
    int tmp_arr[4] = {0};
    for (int i = 0; i < yu; ++i) {
      if (vals[i] >= 128)
        result += vals[i];
    }
    for (int i = yu; i < NUM_ELEMS; i += 16) {
      __m128i cmp1 = _mm_setzero_si128();
      __m128i out1 = _mm_setzero_si128();
      __m128i tmp1 = _mm_loadu_si128((__m128i *)(vals + i));
      cmp1 = _mm_cmpgt_epi32(tmp1, _127);
      out1 = _mm_and_si128(cmp1, tmp1);
      _mm_storeu_si128((__m128i *)tmp_arr, out1);
      result += (tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3]);

      __m128i cmp2 = _mm_setzero_si128();
      __m128i out2 = _mm_setzero_si128();
      __m128i tmp2 = _mm_loadu_si128((__m128i *)(vals + i + 4));
      cmp2 = _mm_cmpgt_epi32(tmp2, _127);
      out2 = _mm_and_si128(cmp2, tmp2);
      _mm_storeu_si128((__m128i *)tmp_arr, out2);
      result += (tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3]);

      __m128i cmp3 = _mm_setzero_si128();
      __m128i out3 = _mm_setzero_si128();
      __m128i tmp3 = _mm_loadu_si128((__m128i *)(vals + i + 8));
      cmp3 = _mm_cmpgt_epi32(tmp3, _127);
      out3 = _mm_and_si128(cmp3, tmp3);
      _mm_storeu_si128((__m128i *)tmp_arr, out3);
      result += (tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3]);

      __m128i cmp4 = _mm_setzero_si128();
      __m128i out4 = _mm_setzero_si128();
      __m128i tmp4 = _mm_loadu_si128((__m128i *)(vals + i + 12));
      cmp4 = _mm_cmpgt_epi32(tmp4, _127);
      out4 = _mm_and_si128(cmp4, tmp4);
      _mm_storeu_si128((__m128i *)tmp_arr, out4);
      result += (tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3]);
    }
    /* Hint: you'll need 1 or maybe 2 tail cases here. */
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}
