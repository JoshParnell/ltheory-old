#include "Diff.h"
#include "Array.h"
#include "AutoClass.h"
#include "AutoPtr.h"
#include "BaseType.h"
#include "Hash.h"

#include <cstring>

struct DiffBlock {
  BASE_TYPE(DiffBlock)

  virtual void GetData(uchar*& pData, Array<uchar> const& source) const = 0;

  FIELDS {}
};

namespace {
  AutoClassDerived(DirectBlock, DiffBlock,
    Vector<uchar>, contents)

    DERIVED_TYPE_EX(DirectBlock)
    DirectBlock() {}

    void GetData(uchar*& pData, Array<uchar> const& source) const {
      memcpy(pData, contents.data(), contents.size());
      pData += contents.size();
    }
  };

  AutoClassDerived(IndirectBlock, DiffBlock,
    size_t, offset,
    size_t, length)

    DERIVED_TYPE_EX(IndirectBlock)
    IndirectBlock() {}

    void GetData(uchar*& pData, Array<uchar> const& source) const {
      memcpy(pData, source.data() + offset, length);
      pData += length;
    }
  };

  size_t MemCmp(
      Array<uchar> const& srcData,
      Array<uchar> const& dstData,
      size_t srcOffset, size_t dstOffset)
  {
    size_t index = 0;
    while (srcOffset < srcData.size() &&
           dstOffset < dstData.size() &&
           srcData[srcOffset] == dstData[dstOffset])
    {
      index++;
      srcOffset++;
      dstOffset++;
    }
    return index;
  }
}

DERIVED_IMPLEMENT(DirectBlock)
DERIVED_IMPLEMENT(IndirectBlock)

namespace LTE {
  struct DiffImpl : public Diff {
    Vector<DiffBlock*> blocks;
    size_t srcSize;
    size_t dstSize;

    ~DiffImpl() {
      blocks.deleteElements();
    }

    Array<uchar>* Inflate(const Array<uchar>& source) {
      if (source.size() != srcSize)
        return NULL;

      Array<uchar>* arr = new Array<uchar>(dstSize);
      uchar* pData = arr->data();
      for (size_t i = 0; i < blocks.size(); ++i)
        blocks[i]->GetData(pData, source);
      return arr;
    }
  };

  AutoPtr<Diff> Diff_Create(
    Array<uchar> const& srcData,
    Array<uchar> const& dstData,
    size_t blockSize)
  {
    const size_t tableSize = 1024 * 17;

    DiffImpl* output = new DiffImpl;
    output->srcSize = srcData.size();
    output->dstSize = dstData.size();
    Vector<size_t> offsetTable[tableSize];

    for (size_t i = blockSize; i < srcData.size(); ++i) {
      size_t index = i - blockSize;
      size_t bucket = Hash(&srcData[index], blockSize) % tableSize;
      offsetTable[bucket] << index;
    }

    Vector<uchar> currentBlock;
    size_t index = 0;
    while (index + blockSize < dstData.size()) {
      size_t bucket = Hash(&dstData[index], blockSize) % tableSize;
      size_t indirectLength = 0;
      size_t indirectOffset;

      for (size_t i = 0; i < offsetTable[bucket].size(); ++i) {
        size_t thisOffset = offsetTable[bucket][i];
        size_t thisLength = MemCmp(srcData, dstData, thisOffset, index);

        if (thisLength >= blockSize) {
          indirectOffset = thisOffset;
          indirectLength = thisLength;
          break;
        }
      }

      if (indirectLength) {
        if (currentBlock.size()) {
          output->blocks << new DirectBlock(currentBlock);
          currentBlock.clear();
        }

        output->blocks << new IndirectBlock(indirectOffset, indirectLength);
        index += indirectLength;
      } else
        for (size_t i = 0; i < blockSize && index < dstData.size(); ++i)
          currentBlock << dstData[index++];
    }

    while (index < dstData.size())
      currentBlock << dstData[index++];

    if (currentBlock.size()) {
      output->blocks << new DirectBlock(currentBlock);
      currentBlock.clear();
    }

    return output;
  }
}
