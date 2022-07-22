#ifndef LTE_Package_h__
#define LTE_Package_h__

#include "Common.h"
#include "Reference.h"

struct PackageT : public RefCounted {
  LT_API ~PackageT();

  LT_API void AddFunction(Function const& function);
  LT_API void AddType(Type const& type);
  LT_API Function GetFunction(String const& name) const;
  LT_API Type GetType(String const& name) const;
};

LT_API Package Package_Create(String const& name);
LT_API Package Package_Global();

#endif
