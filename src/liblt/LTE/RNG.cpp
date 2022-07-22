#include "RNG.h"

namespace {
  struct MTRand {
    MTRand(uint s = 5489U) {
      seed(s);
    }

    MTRand(uint const* arr, size_t size) {
      seed(arr, size);
    }

    void seed(uint s) {
      state[0] = s & 0xFFFFFFFFU;
      for (int i = 1; i < n; ++i) {
        state[i] = 1812433253U * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
        state[i] &= 0xFFFFFFFFU;
      }
      p = n;
    }

    void seed(uint const* array, size_t size) {
      seed(19650218U);
      int i = 1, j = 0;
      for (int k = ((n > (int)size) ? n : (int)size); k; --k) {
        state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525U))
          + array[j] + j;

        state[i] &= 0xFFFFFFFFU;
        ++j;
        j %= size;

        if ((++i) == n) {
          state[0] = state[n - 1];
          i = 1;
        }
      }

      for (int k = n - 1; k; --k) {
        state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941U)) - i;
        state[i] &= 0xFFFFFFFFU;

        if ((++i) == n) {
          state[0] = state[n - 1];
          i = 1;
        }
      }

      state[0] = 0x80000000U;
      p = n;
    }

    uint getNext() {
      if (p == n)
        genState();
      
      uint x = state[p++];
      x ^= (x >> 11);
      x ^= (x <<  7) & 0x9D2C5680U;
      x ^= (x << 15) & 0xEFC60000U;
      return x ^ (x >> 18);
    }

  private:
    static const int n = 624;
    static const int m = 397;

    uint state[n];
    int p;

    uint twiddle(uint u, uint v) {
      return (((u & 0x80000000U) | (v & 0x7FFFFFFFU)) >> 1) ^
             ((v & 1U) ? 0x9908B0DFU : 0x0U);
    }
    
    void genState() {
      for (int i = 0; i < (n - m); ++i)
        state[i] = state[i + m] ^ twiddle(state[i], state[i + 1]);
      for (int i = n - m; i < (n - 1); ++i)
        state[i] = state[i + m - n] ^ twiddle(state[i], state[i + 1]);
      state[n - 1] = state[m - 1] ^ twiddle(state[n - 1], state[0]);
      p = 0;
    }
  };

  struct LCG : public RNGT {
    uint x;

    LCG(uint x) :
      x(x)
    {
      for (int i = 0; i < 5; ++i)
        Increment();
    }

    void Increment() {
      x = (uint)((uint)214013 * x + (uint)2531011);
    }

    uint GetInt() {
      Increment();
      return x;
    }
  };

  struct MTG : public RNGT {
    MTRand mt;

    MTG(uint x) :
      mt(x)
      {}

    MTG(uint const* arr, size_t size) :
      mt(arr, size)
      {}

    uint GetInt() {
      return mt.getNext();
    }
  };

  struct DefaultRNG : public RNGT {
    uint GetInt() {
      return rand() * RAND_MAX + rand();
    }
  };
}

RNG RNG_Default() {
  static RNG drng = new DefaultRNG;
  return drng;
}

RNG RNG_LCG(uint seed) {
  return new LCG(seed);
}

RNG RNG_MTG(uint seed) {
  return new MTG(seed);
}

RNG RNG_MTG(uint const* arr, size_t size) {
  return new MTG(arr, size);
}
