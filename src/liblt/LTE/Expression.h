#ifndef LTE_Expression_h__
#define LTE_Expression_h__

#include "Array.h"
#include "AutoPtr.h"
#include "BaseType.h"
#include "Data.h"
#include "Function.h"
#include "Pointer.h"
#include "Reference.h"

namespace LTE {
  struct CompileEnvironment;
  struct Environment;

  struct ExpressionT : public RefCounted {
    BASE_TYPE(ExpressionT)

    virtual String Emit(Vector<String>& context) const {
      return "";
    }

    virtual void Evaluate(void* returnValue, Environment& env) const = 0;

    virtual void* GetLValue(Environment& env) const {
      return nullptr;
    }

    virtual Type GetType() const = 0;

    virtual bool IsConstant(CompileEnvironment& env) const = 0;

    virtual bool IsLValue() const {
      return false;
    }

    LT_API void Evaluate(void* returnValue) const;

    template <class T>
    void GetResult(T& t) const {
      if (GetType() != Type_Get<T>())
        error("Expression return type differs from expected type")
      Evaluate(&t);
    }

    FIELDS {}
  };

  LT_API Expression Expression_Compile(StringList const& list);

  LT_API Expression Expression_Compile(
    StringList const& list,
    CompileEnvironment& env,
    Vector<String>* locals = nullptr);
}

#endif
