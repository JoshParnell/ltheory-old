#ifndef CodeGen_Variable_h__
#define CodeGen_Variable_h__

#include "CodeBlock.h"

namespace CodeGen {
  struct Variable {
    String type;
    String name;

    Variable(
        String const& type,
        String const& name) :
      type(type),
      name(name)
      {}
  };

  CodeBlock& operator << (CodeBlock& block, Variable const& v) {
    block << v.type << " " << v.name << ";";
    return block;
  }
}

#endif
