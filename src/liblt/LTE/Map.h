#ifndef LTE_Map_h__
#define LTE_Map_h__

#include "Type.h"

#include <map>
#include <cstddef>

template <class KeyT, class ValueT>
struct Map : public NullBase<Map<KeyT, ValueT> > {
  typedef KeyT ArgType;
  typedef NullBase<Map<KeyT, ValueT> > BaseType;
  typedef ValueT* ReturnType;
  typedef typename std::map<KeyT, ValueT>::iterator iterator;
  typedef typename std::map<KeyT, ValueT>::const_iterator const_iterator;

  std::map<KeyT, ValueT> m;

  ValueT& operator[](KeyT const& index) {
    return m[index];
  }

  ValueT const& operator[](KeyT const& index) const {
    return m[index];
  }

  ValueT* operator()(KeyT const& index) {
    return get(index);
  }

  ValueT const* operator()(KeyT const& index) const {
    return get(index);
  }

  iterator begin() {
    return m.begin();
  }

  const_iterator begin() const {
    return m.begin();
  }

  void clear() {
    m.clear();
  }

  bool contains(KeyT const& index) const {
    typename std::map<KeyT, ValueT>::const_iterator it = m.find(index);
    return it != m.end();
  }

  iterator end() {
    return m.end();
  }

  const_iterator end() const {
    return m.end();
  }

  void erase(KeyT const& key) {
    m.erase(key);
  }

  ValueT* get(KeyT const& index) {
    typename std::map<KeyT, ValueT>::iterator it = m.find(index);
    return it == m.end() ? nullptr : &it->second;
  }

  ValueT const* get(KeyT const& index) const {
    typename std::map<KeyT, ValueT>::const_iterator it = m.find(index);
    return it == m.end() ? nullptr : &it->second;
  }

  template <class T>
  iterator random(T& rg) {
    iterator selected = begin();
    size_t i = 0;
    for (iterator it = begin(); it != end(); ++it) {
      if (rg() < 1.f / (float)(++i))
        selected = it;
    }
    return selected;
  }
  

  size_t size() const {
    return m.size();
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, Map const& t) {
    s << "Map(";
    for (const_iterator it = t.begin(); it != t.end(); ++it) {
      s << "\n  ";
      ToStream(s, it->first);
      s << " : ";
      ToStream(s, it->second);
    }
    s << "\n)";
  }

  FIELDS {
    Map* self = (Map*)addr;
    size_t oldSize = self->size();
    size_t sz = oldSize;

    static Type sizeType = Type_Get(sz);
    static Type keyType = Type_Get(*(KeyT const*)0);
    static Type valueType = Type_Get(*(ValueT const*)0);

    m(&sz, "size", sizeType, aux);

    if (sz != oldSize) {
      self->clear();
      KeyT key;
      ValueT value;
      for (size_t i = 0; i < sz; ++i) {
        m(&key, "key", keyType, aux);
        m(&value, "value", valueType, aux);
        (*self)[key] = value;
      }
    } else {
      for (iterator it = self->begin(); it != self->end(); ++it) {
        KeyT& key = (KeyT&)it->first;
        ValueT& value = it->second;
        m(&key, "key", keyType, aux);
        m(&value, "value", valueType, aux);
      }
    }
  }

  DefineMetadataInline(Map)
};

#endif
