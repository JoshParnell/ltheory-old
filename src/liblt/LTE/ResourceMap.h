#ifndef LTE_ResourceMap_h__
#define LTE_ResourceMap_h__

#include "Reference.h"

struct ResourceMapT : public RefCounted {
  virtual ~ResourceMapT() {}

  virtual void AddDirectory(
    String const& path,
    String const& alias = "") = 0;

  virtual void AddFile(
    String const& path,
    String const& alias = "") = 0;

  virtual bool Exists(String const& path) const = 0;

  virtual String Get(String const& path) const = 0;
};

LT_API ResourceMap ResourceMap_Create();

#endif
