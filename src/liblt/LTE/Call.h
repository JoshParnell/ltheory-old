#ifndef LTE_Call_h__
#define LTE_Call_h__

namespace LTE {

  /* Free Function Non-Void */

    template <class RT>
    inline void Call(RT (*fn)(), void** in, void* out) {
      *(RT*)out = fn();
    }

    template <class RT, class T0>
    inline void Call(
      RT (*fn)(T0 const&),
      void** in,
      void* out)
    {
      *(RT*)out = fn(*(T0*)in[0]);
    }

    template <class RT, class T0, class T1>
    inline void Call(
      RT (*fn)(T0 const&, T1 const&),
      void** in,
      void* out)
    {
      *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1]);
    }

    template <class RT, class T0, class T1, class T2>
    inline void Call(
      RT (*fn)(T0 const&, T1 const&, T2 const&),
      void** in,
      void* out)
    {
      *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2]);
    }

  /* Free Function Void. */

    inline void Call(void (*fn)(), void** in, void* out) {
      fn();
    }

    template <class T0>
    inline void Call(
      void (*fn)(T0 const&),
      void** in,
      void* out)
    {
      fn(*(T0*)in[0]);
    }

    template <class T0, class T1>
    inline void Call(
      void (*fn)(T0 const&, T1 const&),
      void** in,
      void* out)
    {
      fn(*(T0*)in[0], *(T1*)in[1]);
    }

    template <class T0, class T1, class T2>
    inline void Call(
      void (*fn)(T0 const&, T1 const&, T2 const&),
      void** in,
      void* out)
    {
      fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2]);
    }

  /* Member Function Non-Void. */

    template <class P, class RT>
    inline void Call(RT (P::*fn)(), void** in, void* out) {
      *(RT*)out = (((P*)in[0])->*fn)();
    }

    template <class P, class RT, class T0>
    inline void Call(
      RT (P::*fn)(T0 const&),
      void** in,
      void* out)
    {
      *(RT*)out = (((P*)in[0])->*fn)(*(T0*)in[1]);
    }

    template <class P, class RT, class T0, class T1>
    inline void Call(
      RT (P::*fn)(T0 const&, T1 const&),
      void** in,
      void* out)
    {
      *(RT*)out = (((P*)in[0])->*fn)(*(T0*)in[1], *(T1*)in[2]);
    }

    template <class P, class RT, class T0, class T1, class T2>
    inline void Call(
      RT (P::*fn)(T0 const&, T1 const&, T2 const&),
      void** in,
      void* out)
    {
      *(RT*)out = (((P*)in[0])->*fn)(*(T0*)in[1], *(T1*)in[2], *(T2*)in[3]);
    }

  /* Member Function Void. */

    template <class P>
    inline void Call(void (P::*fn)(), void** in, void* out) {
      (((P*)in[0])->*fn)();
    }

    template <class P, class T0>
    inline void Call(
      void (P::*fn)(T0 const&),
      void** in,
      void* out)
    {
      (((P*)in[0])->*fn)(*(T0*)in[1]);
    }

    template <class P, class T0, class T1>
    inline void Call(
      void (P::*fn)(T0 const&, T1 const&),
      void** in,
      void* out)
    {
      (((P*)in[0])->*fn)(*(T0*)in[1], *(T1*)in[2]);
    }

    template <class P, class T0, class T1, class T2>
    inline void Call(
      void (P::*fn)(T0 const&, T1 const&, T2 const&),
      void** in,
      void* out)
    {
      (((P*)in[0])->*fn)(*(T0*)in[1], *(T1*)in[2], *(T2*)in[3]);
    }

}

#endif
