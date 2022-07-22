#ifndef LTE_BaseVector_h__
#define LTE_BaseVector_h__

#define DEBUG_BOUNDS_CHECK

#include "Common.h"

namespace LTE {
  template <class T>
  struct BaseVector {
    BaseVector() :
      _data(0),
      _size(0),
      _capacity(0)
      {}

    BaseVector(const BaseVector& other) :
      _data(0)
    {
      _size = other._size;
      _grow(other._capacity);
      for (size_t i = 0; i < _size; ++i)
        _data[i] = other.data[i];
    }

    ~BaseVector() {
      for (size_t i = 0; i < _size; ++i)
        _data[i].~T();
      delete[] ((char*)_data);
    }

    T& operator[](size_t index) {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(index < _size);
#endif
      return _data[index];
    }

    const T& operator[](size_t index) const {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(index < _size);
#endif
      return _data[index];
    }

    BaseVector& operator<<(const T& t) {
      push_back(t);
      return *this;
    }

    T& back() {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(_size > 0);
#endif
      return _data[_size - 1];
    }

    const T& back() const {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(_size > 0);
#endif
      return _data[_size - 1];
    }

    void clear(bool releaseMemory = false) {
      for (size_t i = 0; i < _size; ++i)
        _data[i].~T();
      _size = 0;
      if (releaseMemory) {
        delete[] ((char*)_data);
        _data = 0;
        _capacity = 0;
      }
    }

    T* data() {
      return _data;
    }

    T const* data() const {
      return _data;
    }

    bool empty() const {
      return _size == 0;
    }

    T& front() {
      return _data[0];
    }

    const T& front() const {
      return _data[0];
    }

    void erase(size_t index) {
      _data[index].~T();
      for (size_t i = index; i + 1 < _size; ++i) {
        new (&_data[i]) T (_data[i + 1]);
        _data[i + 1].~T();
      }
      _size--;
    }

    void insert(size_t index, const T& t) {
      if (_size == _capacity)
        _grow(_capacity ? _capacity * 2 : 1);
      for (size_t i = _size; i > index; --i) {
        new (&_data[i]) T (_data[i - 1]);
        _data[i - 1].~T();
      }
      new (&_data[index]) T (t);
      _size++;
    }

    void push_back(const T& t) {
      if (_size == _capacity)
        _grow(_capacity ? _capacity * 2 : 1);
      new (&_data[_size++]) T (t);
    }

    void pop_back() {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(_size > 0);
#endif
      _data[_size--].~T();
    }

    void removeIndex(size_t index) {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(index < _size);
#endif
      if (index + 1 < _size)
        _data[index] = back();
      pop_back();
    }

    void reserve(size_t capacity) {
      if (capacity > _capacity)
        _grow(capacity);
    }

    void resize(size_t newSize, const T& value) {
      if (newSize > _capacity)
        _grow(newSize);
      while (_size < newSize)
        new (&_data[_size++]) T (value);
    }

    size_t size() const {
      return _size;
    }

  private:
    void _grow(size_t target) {
      T* newData = (T*)new char[(_capacity = target) * sizeof(T)];
      for (size_t i = 0; i < _size; ++i)
        new (&newData[i]) T (_data[i]);
      for (size_t i = 0; i < _size; ++i)
        _data[i].~T();
      delete[] ((char*)_data);
      _data = newData;
    }

    T* _data;
    size_t _size, _capacity;
  };
}

#endif
