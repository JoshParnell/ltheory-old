#include "../Expressions.h"

#include "LTE/StringList.h"

namespace LTE {
  Expression Expression_List(StringList const& list, CompileEnvironment& env) {
    StringList newList = list->Clone();
    ((StringListList*)newList.t)->elements.eraseIndex(0);
    return Expression_Constant(newList);
  }
}
