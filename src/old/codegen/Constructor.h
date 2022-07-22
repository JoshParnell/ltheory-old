#ifndef CodeGen_Constructor_h__
#define CodeGen_Constructor_h__

#include "CodeBlock.h"
#include "Variable.h"

namespace CodeGen {
  struct Initializer {
    String name;
    String value;

    Initializer(
        String const& name,
        String const& value) :
      name(name),
      value(value)
      {}
  };

  struct Constructor {
    String name;
    Vector<Variable> params;
    Vector<Initializer> initializers;
    CodeBlock block;

    Constructor(
        String const& name,
        CodeBlock const& block) :
      name(name),
      block(block)
      {}

    Constructor(
        String const& name,
        Vector<Variable> const& params,
        Vector<Initializer> const& initializers,
        CodeBlock const& block) :
      name(name),
      params(params),
      initializers(initializers),
      block(block)
      {}
  };

  CodeBlock& operator << (CodeBlock& block, Constructor const& c) {
    String suffix = c.initializers.size() ? ") :" : ")";

    block << c.name << "(" << (c.params.size() ? "" : suffix);

    if (c.params.size())
      for (size_t i = 0; i < c.params.size(); ++i)
        block << kIndent << kIndent
              << c.params[i].type << " "
              << c.params[i].name
              << (i + 1 < c.params.size() ? "," : suffix);

    if (c.initializers.size())
      for (size_t i = 0; i < c.initializers.size(); ++i)
        block << kIndent
              << c.initializers[i].name << "("
              << c.initializers[i].value << ")"
              << (i + 1 < c.initializers.size() ? "," : "");

    if (c.block.IsEmpty())
      block << kIndent << "{}";
    else {
      CodeBlock thisBlock = c.block;
      thisBlock.Indent();
      block << "{";
      block << thisBlock;
      block << "}";
    }

    block.NewLine();
    return block;
  }
}

#endif
