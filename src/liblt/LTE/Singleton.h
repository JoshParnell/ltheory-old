#ifndef LTE_Singleton_h__
#define LTE_Singleton_h__

#include "AutoPtr.h"

namespace LTE {
  template <
    class T,
    T* (*createFn)(),
    template <class> class container = AutoPtr>

  struct Singleton {
    static T& Get() {
      static container<T> instance;
      if (!instance)
        instance = createFn();
      return *instance;
    }
  };
}

#endif
