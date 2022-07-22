#ifndef LTE_Pushable_h__
#define LTE_Pushable_h__

#include "Pointer.h"
#include "Vector.h"

namespace LTE {
  template <class T>
  struct Pushable {
    Pushable() {
      if (GetStack().empty())
        Push();
    }

    ~Pushable() {
      Pop();
    }

    static Pointer<T> Get() {
      return GetStack().size() ? GetStack().back() : nullptr;
    }

    T* Pop() {
      GetStack().remove((T*)this);
      return (T*)this;
    }

    T* Push() {
      LTE_ASSERT(GetStack().size() < 128);
      GetStack().push((T*)this);
      return (T*)this;
    }

  private:
    static Vector<Pointer<T> >& GetStack() {
      static Vector<Pointer<T> > active;
      return active;
    }
  };
}

#endif
