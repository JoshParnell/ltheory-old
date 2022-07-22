#ifndef LTE_Distribution_h__
#define LTE_Distribution_h__

#include "VectorMap.h"

template <class T>
struct Distribution : public VectorMap<T, float> {
  typedef typename VectorMap<T, float>::BaseType BaseType;
  typedef Distribution SelfType;

  Distribution() {}

  Distribution(Distribution const& other) :
    VectorMap<T, float>(other)
    {}

  Distribution& operator()(T const& t, float weight) {
    add(t, weight);
    return *this;
  }

  void add(T const& t, float weight) {
    (*this)[t] += weight;
  }

  float getTotal() const {
    float total = 0.0f;
    for (size_t i = 0; i < this->entries.size(); ++i)
      total += this->entries[i].value;
    return total;
  }

  size_t getMinIndex() const {
    size_t minIndex = 0;
    for (size_t i = 1; i < this->entries.size(); ++i)
      if (this->entries[i].value < this->entries[minIndex].value)
        minIndex = i;
    return minIndex;
  }
  
  T const& getMin() const {
    return (*this)[getMinIndex()];
  }

  void popMin() {
    LTE_ASSERT(this->size() > 0);
    this->removeIndex(getMinIndex());
  }

  T const& sample(float d) const {
    /* TODO : Implement alias method for faster sampling. */
    float cumulative = getTotal();
    float chosen = cumulative * d;
    cumulative = 0.0f;
    for (size_t i = 0; i < this->entries.size(); ++i) {
      cumulative += this->entries[i].value;
      if (chosen <= cumulative)
        return this->entries[i].key;
    }
    return this->entries.back().key;
  }

  template <class StreamT>
  friend void _ToStream(StreamT& stream, Distribution const& self) {
    stream << "Distribution()";
  }

  AUTOMATIC_REFLECTION_PARAMETRIC1(Distribution, T)
};

#endif
