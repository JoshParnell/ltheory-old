#ifndef LTE_HashMap_h__
#define LTE_HashMap_h__

#include <functional>

#if defined(LIBLT_WINDOWS)
  #include <unordered_map>

  template <class KeyT, class ValueT, class HasherT>
  struct HashMapT {
    typedef std::unordered_map<KeyT, ValueT, HasherT> result;
  };

  template <class T>
  inline size_t DefaultHash(T const& t) {
    return std::hash<T>(t);
  }

  template <class T>
  struct DefaultHasher : public std::hash<T> {};

#elif defined(LIBLT_MACOS)
  #include <unordered_map>

  template <class KeyT, class ValueT, class HasherT>
  struct HashMapT {
    typedef std::unordered_map<KeyT, ValueT, HasherT> result;
  };

  template <class T>
  inline size_t DefaultHash(T const& t) {
    return std::hash<T>(t);
  }

  template <class T>
  struct DefaultHasher : public std::hash<T> {};

#else
  #include <tr1/unordered_map>

  template <class KeyT, class ValueT, class HasherT>
  struct HashMapT {
    typedef std::tr1::unordered_map<KeyT, ValueT, HasherT> result;
  };

  template <class T>
  inline size_t DefaultHash(T const& t) {
    return std::tr1::hash<T>(t);
  }

  template <class T>
  struct DefaultHasher : public std::tr1::hash<T> {};

#endif

namespace LTE {
  template <class KeyT, class ValueT, class HasherT = DefaultHasher<KeyT> >
  class HashMap : public HashMapT<KeyT, ValueT, HasherT>::result {
  public:
    typedef typename HashMapT<KeyT, ValueT, HasherT>::result BaseT;
    typedef typename BaseT::iterator iterator;
    typedef typename BaseT::const_iterator const_iterator;

    bool contains(KeyT const& key) const {
      return this->find(key) != this->end();
    }

    ValueT* get(KeyT const& index) {
      typename BaseT::iterator it = this->find(index);
      return it == this->end() ? nullptr : &it->second;
    }

    ValueT const* get(KeyT const& index) const {
      typename BaseT::const_iterator it = this->find(index);
      return it == this->end() ? nullptr : &it->second;
    }

    template <class StreamT>
    friend void _ToStream(StreamT& s, HashMap const& t) {
      s << "HashMap(";
      for (const_iterator it = t.begin(); it != t.end(); ++it) {
        s << "\n  ";
        ToStream(s, it->first);
        s << " : ";
        ToStream(s, it->second);
      }
      s << "\n)";
    }
  };
}

#endif
