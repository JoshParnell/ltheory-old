#ifndef LTE_Vector_h__
#define LTE_Vector_h__

#include "Function.h"
#include <vector>

template <class T>
struct Vector : public NullBase<Vector<T> > {
  typedef Vector SelfType;
  typedef NullBase<Vector<T> > BaseType;
  typedef size_t ArgType;
  typedef T ReturnType;
  typedef typename std::vector<T>::iterator IteratorType;
  typedef typename std::vector<T>::const_iterator CIteratorType;

  std::vector<T> v;

  Vector() {}

  Vector(std::vector<T> const& v) :
    v(v)
    {}

  Vector(T const& elem) {
    v.push_back(elem);
  }

  Vector(T const& e1, T const& e2) {
    v.reserve(2);
    v.push_back(e1);
    v.push_back(e2);
  }

  Vector(T const& e1, T const& e2, T const& e3) {
    v.reserve(3);
    v.push_back(e1);
    v.push_back(e2);
    v.push_back(e3);
  }

  operator std::vector<T> const&() const {
    return v;
  }

  operator std::vector<T>&() {
    return v;
  }

  T& operator[](size_t index) {
    return get(index);
  }

  T const& operator[](size_t index) const {
    return get(index);
  }

  T& operator()(size_t index) {
    return get(index);
  }

  T const& operator()(size_t index) const {
    return get(index);
  }

  Vector& operator=(T const& elem) {
    v.clear();
    v.push_back(elem);
    return *this;
  }

  void append(T const& element) {
    v.push_back(element);
  }

  void append(Vector const& other) {
    if (other.empty())
      return;
    v.reserve(v.size() + other.size());
    for (size_t i = 0; i < other.size(); ++i)
      v.push_back(other[i]);
  }

  T& back() {
    return v.back();
  }

  T const& back() const {
    return v.back();
  }

  IteratorType begin() {
    return v.begin();
  }

  CIteratorType begin() const {
    return v.begin();
  }

  void clear() {
    v.clear();
  }

  template <class T2>
  bool contains(T2 const& t) const {
    for (size_t i = 0; i < v.size(); ++i)
      if (v[i] == t)
        return true;
    return false;
  }

  T* data() {
    return &front();
  }

  T const* data() const {
    return &front();
  }

  void deleteElements() {
    for (size_t i = 0; i < v.size(); ++i)
      delete v[i];
    clear();
  }

  bool empty() const {
    return v.empty();
  }

  IteratorType end() {
    return v.end();
  }

  CIteratorType end() const {
    return v.end();
  }

  template <class T2>
  void erase(T2 const& elem) {
    for (size_t i = 0; i < v.size(); ++i) {
      if (v[i] == elem) {
        eraseIndex(i);
        return;
      }
    }
  }

  void eraseIndex(size_t index) {
    v.erase(v.begin() + index);
  }

  T& front() {
#ifdef DEBUG_BOUNDS_CHECK
    LTE_ASSERT(v.size());
#endif
    return v[0];
  }

  T const& front() const {
#ifdef DEBUG_BOUNDS_CHECK
    LTE_ASSERT(v.size());
#endif
    return v[0];
  }

  T& get(size_t index) {
#ifdef DEBUG_BOUNDS_CHECK
    LTE_ASSERT(index < v.size());
#endif
    return v[index];
  }

  T const& get(size_t index) const {
#ifdef DEBUG_BOUNDS_CHECK
    LTE_ASSERT(index < v.size());
#endif
    return v[index];
  }

  T& getModulo(size_t index) {
    LTE_ASSERT(v.size());
    return v[index % v.size()];
  }

  T const& getModulo(size_t index) const {
    LTE_ASSERT(v.size());
    return v[index % v.size()];
  }

  void resize(size_t size, T const& t) {
    v.resize(size, t);
  }

  size_t size() const {
    return v.size();
  }

  void insert(size_t index, T const& t) {
    LTE_ASSERT(index <= v.size());
    v.insert(v.begin() + index, t);
  }

  bool isEmpty() const {
    return v.empty();
  }

  Vector& operator<<(T const& t) {
    push(t);
    return *this;
  }

  Vector& operator>>(T& t) {
    LTE_ASSERT(v.size());
    t = v.back();
    v.pop_back();
    return *this;
  }

  void push(T const& t) {
    append(t);
  }

  void pushEmpty() {
    append(T());
  }

  void pushFront(T const& t) {
    v.insert(v.begin(), t);
  }

  void pop() {
    v.pop_back();
  }

  void popFront() {
    v.erase(v.begin());
  }

  T& random() {
    LTE_ASSERT(v.size());
    return v[rand() % v.size()];
  }

  T const& random() const {
    LTE_ASSERT(v.size());
    return v[rand() % v.size()];
  }

  template <class T2>
  bool remove(T2 const& elem) {
    for (size_t i = 0; i < v.size(); ++i) {
      if (v[i] == elem) {
        removeIndex(i);
        return true;
      }
    }
    return false;
  }

  void removeIndex(size_t index) {
    LTE_ASSERT(index < v.size());
    if (index + 1 < v.size())
      v[index] = v.back();
    v.pop_back();
  }

  void reserve(size_t capacity) {
    v.reserve(capacity);
  }

  Vector& shuffle() {
    for (size_t i = 0; i < v.size() - 1; ++i)
      std::swap(v[i], v[i + 1 + rand() % (v.size() - (i + 1))]);
    return *this;
  }

  FIELDS {
    Vector* self = (Vector*)addr;
    size_t oldSize = self->size();
    size_t sz = oldSize;

    m(&sz, "size", Type_Get(sz), aux);

    if (sz != oldSize)
      self->resize(sz, T());
    
    for (size_t i = 0; i < sz; ++i)
      m(&(*self)[i], "data", Type_Get<T>(), aux);
  }

  VoidMemberFunction(Append,
    "Append 'element' to the back of the vector",
    T, element)
  {
    this->push(element);
  }

  MemberFunction(T, Get,
    "Return the element at 'index' in the vector",
    int, index)
  {
    return (*this)[index];
  }

  VoidMemberFunction(Set,
    "Set the number of elements in the vector",
    int, index,
    T, element)
  {
    (*this)[index] = element;
  }

  MemberFunctionNoParams(int, Size,
    "Return the number of elements in the vector")
  {
    return (int)this->size();
  }

  METADATA {
    MEMBERFUNCTION(Append)
    MEMBERFUNCTION(Get)
    MEMBERFUNCTION(Size)
    MEMBERFUNCTION(Set)
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, Vector const& v) {
    s << "Vector(";
    for (size_t i = 0; i < v.size(); ++i) {
      if (i)
        s << ", ";
      ToStream(s, v[i]);
    }
    s << ")";
  }

  AUTOMATIC_REFLECTION_PARAMETRIC1(Vector, T)
};

#endif
