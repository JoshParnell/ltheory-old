#ifndef LTE_VectorMap
#define LTE_VectorMap

#include "Vector.h"

template <class KeyT, class ValueT>
struct VectorMapEntry {
  KeyT key;
  ValueT value;

  VectorMapEntry() {}
  VectorMapEntry(KeyT const& key) :
    key(key),
    value(ValueT())
    {}
};

template <class KeyT, class ValueT>
struct VectorMap : public NullBase<VectorMap<KeyT, ValueT> > {
  typedef KeyT ArgType;
  typedef NullBase<VectorMap<KeyT, ValueT> > BaseType;

  Vector<VectorMapEntry<KeyT, ValueT> > entries;

  ValueT& operator[](KeyT const& key) {
    for (size_t i = 0; i < entries.size(); ++i)
      if (entries[i].key == key)
        return entries[i].value;
    entries << VectorMapEntry<KeyT, ValueT>(key);
    return entries.back().value;
  }

  ValueT const& operator[](KeyT const& key) const {
    return ((const VectorMap*)this)->operator[](key);
  }

  void clear() {
    entries.clear();
  }

  bool contains(KeyT const& key) const {
    for (size_t i = 0; i < entries.size(); ++i)
      if (entries[i].key == key)
        return true;
    return false;
  }

  VectorMapEntry<KeyT, ValueT>* find(KeyT const& key) {
    for (size_t i = 0; i < entries.size(); ++i)
      if (entries[i].key == key)
        return &entries[i];
    return nullptr;
  }

  const VectorMapEntry<KeyT, ValueT>* find(KeyT const& key) const {
    return ((VectorMap*)this)->find(key);
  }

  ValueT* get(KeyT const& key) {
    for (size_t i = 0; i < entries.size(); ++i)
      if (entries[i].key == key)
        return &entries[i].value;
    return nullptr;
  }

  ValueT const* get(KeyT const& key) const {
    return ((VectorMap*)this)->get(key);
  }

  void getDomain(Vector<KeyT>& v) const {
    for (size_t i = 0; i < entries.size(); ++i)
      v.push(entries[i].key);
  }

  KeyT& getIndexKey(size_t index) {
    return entries[index].key;
  }

  KeyT const& getIndexKey(size_t index) const {
    return entries[index].key;
  }

  ValueT& getIndexValue(size_t index) {
    return entries[index].value;
  }

  ValueT const& getIndexValue(size_t index) const {
    return entries[index].value;
  }

  KeyT* getKey(ValueT const& value) {
    for (size_t i = 0; i < entries.size(); ++i)
      if (entries[i].value == value)
        return &entries[i].key;
    return nullptr;
  }

  KeyT const* getKey(ValueT const& value) const {
    return ((VectorMap*)this)->getKey(value);
  }

  bool remove(KeyT const& key) {
    for (size_t i = 0; i < entries.size(); ++i) {
      if (entries[i].key == key) {
        if (i + 1 < entries.size())
          entries[i] = entries.back();
        entries.pop();
        return true;
      }
    }
    return false;
  }

  void removeIndex(size_t index) {
    entries.removeIndex(index);
  }

  size_t size() const {
    return entries.size();
  }

  FIELDS {
    VectorMap* self = (VectorMap*)addr;
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
      for (size_t i = 0; i < self->entries.size(); ++i) {
        KeyT& key = self->entries[i].key;
        ValueT& value = self->entries[i].value;
        m(&key, "key", keyType, aux);
        m(&value, "value", valueType, aux);
      }
    }
  }

  DefineMetadataInline(VectorMap)
};

#endif
