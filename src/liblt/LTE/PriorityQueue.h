#ifndef LTE_PriorityQueue_h__
#define LTE_PriorityQueue_h__

#include "Tuple.h"
#include "Vector.h"

namespace LTE {
  template <class T>
  struct PriorityQueue {
    typedef Tuple2<T, float> Element;
    Vector<Element> elements;

    float& operator[](T const& element) {
      for (size_t i = 0; i < elements.size(); ++i)
        if (elements[i].x == element)
          return elements[i].y;
      elements.push(Tuple(element, 0));
      return elements.back().y;
    }

    float& getIndex(size_t index) {
      return elements[index].y;
    }

    void popMin() {
      LTE_ASSERT(size() > 0);
      size_t minIndex = 0;
      for (size_t i = 1; i < elements.size(); ++i)
        if (elements[i].y < elements[minIndex].y)
          minIndex = i;
      elements.removeIndex(minIndex);
    }

    T sample(float t) const {
      LTE_ASSERT(size() > 0);
      float total = 0;
      for (size_t i = 0; i < elements.size(); ++i)
        total += elements[i].y;

      float sampleIndex = t * total;
      total = 0;

      for (size_t i = 0; i < elements.size(); ++i) {
        total += elements[i].y;
        if (total >= sampleIndex)
          return elements[i].x;
      }
    
      return elements.back().x;
    }

    size_t size() const {
      return elements.size();
    }
  };

  template <class StreamT, class T>
  void _ToStream(StreamT& s, PriorityQueue<T>& t) {
    ToStream(s, t.elements);
  }
}

#endif
