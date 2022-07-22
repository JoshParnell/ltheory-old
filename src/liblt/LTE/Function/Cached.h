#ifndef LTE_Function_Cached_h__
#define LTE_Function_Cached_h__

#include "Wrapper.h"
#include "LTE/Location.h"
#include "LTE/Serializer.h"

namespace LTE {
  template <class T>
  struct CachedT {
    typedef typename T::ReturnType ReturnType;

    T t;
    ReturnType result;
    bool cached;

    CachedT(T const& t) :
      t(t),
      cached(false)
      {}

    ReturnType operator()() {
      if (!cached) {
        cached = true;
        result = t();
      }
      return result;
    }
  };

  template <class T>
  struct DiskCachedT {
    typedef typename T::ReturnType ReturnType;

    T t;
    ReturnType result;
    String cacheFile;
    bool cached;
    
    DiskCachedT(T const& t, String const& cacheFile) :
      t(t),
      cacheFile(cacheFile),
      cached(false)
      {}

    ReturnType operator()() {
      if (!cached) {
        cached = true;

        Location cache = Location_Cache(cacheFile + ".bin");
        if (!cache->Exists() || !LoadFrom(result, cache, 0, 0)) {
          result = t();
          SaveTo(result, cache, 0);
        }
      }
      return result;
    }
  };

  template <class T>
  WrapperT<CachedT<T>, typename CachedT<T>::ReturnType>
  Cached(T const& t) {
    return Wrapper(CachedT<T>(t));
  }

  template <class T>
  WrapperT<DiskCachedT<T>, typename DiskCachedT<T>::ReturnType>
  DiskCached(T const& t, String const& cacheFile) {
    return Wrapper(DiskCachedT<T>(t, cacheFile));
  }
}

#endif
