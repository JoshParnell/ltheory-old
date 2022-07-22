#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Pool.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionConstant, ExpressionT,
    Data, value)
    DERIVED_TYPE_EX(ExpressionConstant)
    POOLED_TYPE
    
    ExpressionConstant() {}

    String Emit(Vector<String>& context) const {
#define X(x)                                                                   \
      if (value.type == Type_Get<x>())                                         \
        return LTE::ToString(*(x const*)value.data);
      PRIMITIVE_TYPE_X
#undef X
      return "<Constant>";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      value.type->Assign(value.data, returnValue);
    }

    Type GetType() const {
      return value.type;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return true;
    }
  };
}

namespace LTE {
  Expression Expression_Constant(Data const& value) {
    return new ExpressionConstant(value);
  }

  Expression Expression_Constant(
    StringList const& list,
    CompileEnvironment& env)
  {
    bool alpha = false;
    uint decimal = 0;
    bool digit = false;

    String const& value = list->GetValue();
    for (size_t i = 0; i < value.size(); ++i) {
      char c = value[i];
      if (isalpha(c))
        alpha = true;
      if (isdigit(c) || c == '-')
        digit = true;
      if (c == '.')
        decimal++;
    }

    if (value == "true")
      return Expression_Constant(true);

    if (value == "false")
      return Expression_Constant(false);

    if (value.size() >= 2 &&
        value.front() == '"' &&
        value.back() == '"')
    {
      String str = value.substr(1, value.size() - 2);
      return Expression_Constant(str);
    }

    if (value.size() >= 2 &&
        value.front() == '\'' &&
        value.back() == '\'')
    {
      String str = value.substr(1, value.size() - 2);
      return Expression_Constant(str);
    }

    /* Integer. */
    if (!alpha && digit && decimal == 0)
      return Expression_Constant(FromString<int>(value));

    /* Float. */
    if (!alpha && digit && decimal == 1)
      return Expression_Constant(FromString<float>(value));

    return nullptr;
  }
}
