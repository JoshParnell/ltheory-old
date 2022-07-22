#ifndef Strukt_CodeObject_h__
#define Strukt_CodeObject_h__

#include "Common.h"

#include "LTE/BaseType.h"
#include "LTE/Reference.h"

#include "UI/Common.h"

struct CodeObjectT : public RefCounted {
  BASE_TYPE_EX(CodeObjectT)

  CodeObjectT() {}
  virtual ~CodeObjectT() {}

  virtual String GetName() const = 0;

  virtual String GetType() const = 0;

  virtual Widget GetWidget() const = 0;

  FIELDS {}
};

#endif
