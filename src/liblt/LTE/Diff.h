#ifndef LTE_Diff_h__
#define LTE_Diff_h__

#include "Vector.h"

namespace LTE {
  struct Diff {
    virtual ~Diff() {}

    virtual Array<uchar>* Inflate(Array<uchar> const& source) = 0;
  };

  LT_API AutoPtr<Diff> Diff_Create(
    Array<uchar> const& srcData,
    Array<uchar> const& dstData,
    size_t blockSize = 16);
}

#endif
