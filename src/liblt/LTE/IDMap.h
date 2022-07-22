#ifndef LTE_IDMap_h__
#define LTE_IDMap_h__

#include "Map.h"

template <class T>
struct IDMap {
  size_t index;
  Map<T, size_t> elements;

  IDMap() : index(0) {}

  size_t Get(T const& element) {
    size_t* ptr = elements.get(element);
    if (ptr)
      return *ptr;

    size_t value = index++;
    elements[element] = value;
    return value;
  }
};

#endif
