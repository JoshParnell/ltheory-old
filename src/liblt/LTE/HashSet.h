#ifndef LTE_HashSet_h__
#define LTE_HashSet_h__

#if defined(LIBLT_WINDOWS)
  #include <unordered_set>
  template <class KeyT>
  struct HashSetT {
    typedef std::unordered_set<KeyT> result;
  };

#elif defined(LIBLT_MACOS)

  #include <unordered_set>

  template <class KeyT>
  struct HashSetT {
    typedef std::unordered_set<KeyT> result;
  };

#else

  #include <tr1/unordered_set>

  template <class KeyT>
  struct HashSetT {
    typedef std::tr1::unordered_set<KeyT> result;
  };


#endif

namespace LTE {
  template <class KeyT>
  struct HashSet : public HashSetT<KeyT>::result {
    bool contains(KeyT const& key) const {
      return this->find(key) != this->end();
    }
  };
}

#endif
