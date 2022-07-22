#ifndef LTE_Archive_h__
#define LTE_Archive_h__

#include "String.h"

namespace LTE {
  struct Archive {
    virtual ~Archive() {}

    LT_API static Archive* Open(String const& filename);

    virtual bool Contains(String const& filename) = 0;
    virtual void Dump() const = 0;
    virtual size_t GetFileCount() const = 0;
    virtual String GetFileName(size_t index) const = 0;
    virtual Array<uchar>* Read(String const& filename) = 0;
    virtual bool Write(String const& filename, Array<uchar> const& data) = 0;
  };
}

#endif
