#ifndef LTE_VecBase_h__
#define LTE_VecBase_h__

#include "Common.h"
#include "Type.h"
#include "StdMath.h"

namespace LTE {
  template <class T, int N>
  struct DefaultVecStorage {
    typedef DefaultVecStorage SelfType;

    T e[N];

    T& operator[](int index) {
      return e[index];
    }

    T const& operator[](int index) const {
      return e[index];
    }

    FIELDS {
      SelfType* self = (SelfType*)addr;
      static Type type1 = Type_Get(*(T*)0);
      for (int i = 0; i < N; ++i)
        m(&((*self)[i]), "data", type1, aux);
    }
  };

  template <class T, int N, class VecStorageT = DefaultVecStorage<T, N> >
  struct Vec : public VecStorageT {
    typedef VecStorageT BaseType;
    typedef T ReturnType;
    typedef Vec SelfType;

    Vec() {}

    Vec(T const& t) {
      for (int i = 0; i < N; ++i)
        (*this)[i] = t;
    }

    template <class U>
    Vec(Vec<U, N> const& v) {
      for (int i = 0; i < N; ++i)
        (*this)[i] = (T)v[i];
    }

    Vec& operator=(Vec const& other) {
      for (int i = 0; i < N; ++i)
        (*this)[i] = other[i];
      return *this;
    }

    bool operator==(Vec const& v) const {
      for (int i = 0; i < N; ++i)
        if ((*this)[i] != v[i])
          return false;
      return true;
    }

    bool operator!=(Vec const& v) const {
      for (int i = 0; i < N; ++i)
        if ((*this)[i] != v[i])
          return true;
      return false;
    }

    template <class T2, class ST2>
    Vec& operator+=(Vec<T2, N, ST2> const& v) {
      for (int i = 0; i < N; ++i)
        (*this)[i] += v[i];
      return *this;
    }

    template <class T2, class ST2>
    Vec& operator-=(Vec<T2, N, ST2> const& v) {
      for (int i = 0; i < N; ++i)
        (*this)[i] -= v[i];
      return *this;
    }

    template <class T2, class ST2>
    Vec& operator*=(Vec<T2, N, ST2> const& v) {
      for (int i = 0; i < N; ++i)
        (*this)[i] *= v[i];
      return *this;
    }

    template <class T2, class ST2>
    Vec& operator/=(Vec<T2, N, ST2> const& v) {
      for (int i = 0; i < N; ++i)
        (*this)[i] /= v[i];
      return *this;
    }

    template <class T2>
    Vec& operator*=(T2 const& t) {
      for (int i = 0; i < N; ++i)
        (*this)[i] *= t;
      return *this;
    }

    template <class T2>
    Vec& operator/=(T2 const& t) {
      for (int i = 0; i < N; ++i)
        (*this)[i] /= t;
      return *this;
    }

    Vec operator-() const {
      Vec v;
      for (int i = 0; i < N; ++i)
        v[i] = -(*this)[i];
      return v;
    }

    template <class T2, class ST2>
    friend Vec operator+(Vec const& v1, Vec<T2, N, ST2> const& v2) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] += v2[i];
      return result;
    }

    template <class T2, class ST2>
    friend Vec operator-(Vec const& v1, Vec<T2, N, ST2> const& v2) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] -= v2[i];
      return result;
    }

    template <class T2, class ST2>
    friend Vec operator*(Vec const& v1, Vec<T2, N, ST2> const& v2) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] *= v2[i];
      return result;
    }

    template <class T2, class ST2>
    friend Vec operator/(Vec const& v1, Vec<T2, N, ST2> const& v2) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] /= v2[i];
      return result;
    }

    friend Vec operator*(Vec const& v1, T const& s) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] *= s;
      return result;
    }

    friend Vec operator/(Vec const& v1, T const& s) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] /= s;
      return result;
    }

    friend Vec operator*(T const& s, Vec const& v1) {
      Vec result = v1;
      for (int i = 0; i < N; ++i)
        result[i] *= s;
      return result;
    }

    friend Vec operator/(T const& s, Vec const& v1) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = v1[i] / s;
      return result;
    }

    T GetAverage() const {
      return GetSum() / N;
    }

    T GetGeometricAverage() const {
      return Pow(GetProduct(), 1 / (T)N);
    }

    T GetLength() const {
      return Sqrt(GetLengthSquared());
    }

    T GetLengthSquared() const {
      return Dot(*this, *this);
    }

    T GetMax() const {
      T t = (*this)[0];
      for (int i = 1; i < N; ++i)
        if ((*this)[i] > t)
          t = (*this)[i];
      return t;
    }

    T GetMin() const {
      T t = (*this)[0];
      for (int i = 1; i < N; ++i)
        if ((*this)[i] < t)
          t = (*this)[i];
      return t;
    }

    T GetProduct() const {
      T product = 1;
      for (int i = 0; i < N; ++i)
        product *= (*this)[i];
      return product;
    }

    size_t GetSize() const {
      return N;
    }
  
    T GetSum() const {
      T sum = 0;
      for (int i = 0; i < N; ++i)
        sum += (*this)[i];
      return sum;
    }

    friend Vec Abs(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Abs(v[i]);
      return result;
    }

    friend Vec Ceil(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Ceil(v[i]);
      return result;
    }

    friend Vec Clamp(Vec const& v, Vec const& lower, Vec const& upper) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Clamp(v[i], lower[i], upper[i]);
      return result;
    }

    friend Vec Exp(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Exp(v[i]);
      return result;
    }

    friend T Dot(Vec const& v1,
          Vec const& v2)
    {
      return (v1 * v2).GetSum();
    }

    friend Vec Floor(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Floor(v[i]);
      return result;
    }

    friend Vec Fract(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Fract(v[i]);
      return result;
    }

    friend T Length(Vec const& v) {
      return Sqrt(Dot(v, v));
    }

    friend Vec Log(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Log(v[i]);
      return result;
    }

    friend Vec Max(Vec const& v1, Vec const& v2) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Max(v1[i], v2[i]);
      return result;
    }

    friend Vec Min(Vec const& v1, Vec const& v2) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Min(v1[i], v2[i]);
      return result;
    }

    friend Vec Mix(Vec const& v1, Vec const& v2, Vec const& t) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Mix(v1[i], v2[i], t[i]);
      return result;
    }

    friend Vec Normalize(Vec const& v) {
      return v / Length(v);
    }

    friend Vec Pow(Vec const& v, T const& s) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Pow(v[i], s);
      return result;
    }

    friend Vec Round(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Round(v[i]);
      return result;
    }

    friend Vec Saturate(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Saturate(v[i]);
      return result;
    }

    friend Vec Sign(Vec const& v) {
      Vec result;
      for (int i = 0; i < N; ++i)
        result[i] = Sign(v[i]);
      return result;
    }

    FIELDS {
      VecStorageT::MapFields(type, addr, m, aux);
    }

    template <class StreamT>
    friend void _ToStream(StreamT& s, Vec const& v) {
      ToStream(s, "Vec");
      ToStream(s, N);
      ToStream(s, '(');
      for (int i = 0; i < N; ++i) {
        if (i)
          ToStream(s, ',');
        ToStream(s, v[i]);
      }
      ToStream(s, ')');
    }

    DefineMetadataInline(Vec)
  };
}

#endif
