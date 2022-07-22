#ifndef LTE_Tuple_h__
#define LTE_Tuple_h__

#include "AutoClass.h"

template <class T1, class T2>
AutoClass(Tuple2,
  T1, x,
  T2, y)

  Tuple2() {}

  template <class NT1, class NT2>
  operator Tuple2<NT1, NT2>() const {
    return Tuple2<NT1, NT2>(static_cast<NT1>(x), static_cast<NT2>(y));
  }

  friend bool operator==(Tuple2 const& a, Tuple2 const& b) {
    return a.x == b.x && a.y == b.y;
  }

  friend bool operator!=(Tuple2 const& a, Tuple2 const& b) {
    return a.x != b.x || a.y != b.y;
  }

  friend bool operator< (Tuple2 const& a, Tuple2 const& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
  }

  size_t size() const {
    return 2;
  }
};

template <class T1, class T2, class T3>
AutoClass(Tuple3,
  T1, x,
  T2, y,
  T3, z)

  Tuple3() {}

  template <class NT1, class NT2, class NT3>
  operator Tuple3<NT1, NT2, NT3>() const {
    return Tuple3<NT1, NT2, NT3>(static_cast<NT1>(x), static_cast<NT2>(y),
                                 static_cast<NT3>(z));
  }

  friend bool operator==(Tuple3 const& a, Tuple3 const& b) {
    return a.x == b.x && a.y == b.y && a.z == a.z;
  }

  friend bool operator!=(Tuple3 const& a, Tuple3 const& b) {
    return a.x != b.x || a.y != b.y || a.z != a.z;
  }

  size_t size() const {
    return 3;
  }
};

template <class T1, class T2, class T3, class T4>
AutoClass(Tuple4,
  T1, x,
  T2, y,
  T3, z,
  T4, w)

  Tuple4() {}

  friend bool operator==(Tuple4 const& a, Tuple4 const& b) {
    return a.x == b.x && a.y == b.y && a.z == a.z && a.w == b.w;
  }

  friend bool operator!=(Tuple4 const& a, Tuple4 const& b) {
    return a.x != b.x || a.y != b.y || a.z != a.z || a.w != b.w;
  }

  size_t size() const {
    return 4;
  }
};

template <class T1, class T2>
Tuple2<T1, T2>
Tuple(T1 const& x, T2 const& y) {
  return Tuple2<T1, T2>(x, y);
}

template <class T1, class T2, class T3>
Tuple3<T1, T2, T3>
Tuple(T1 const& x, T2 const& y, T3 const& z) {
  return Tuple3<T1, T2, T3>(x, y, z);
}

template <class T1, class T2, class T3, class T4>
Tuple4<T1, T2, T3, T4>
Tuple(T1 const& x, T2 const& y, T3 const& z, T4 const& w) {
  return Tuple4<T1, T2, T3, T4>(x, y, z, w);
}

#endif
