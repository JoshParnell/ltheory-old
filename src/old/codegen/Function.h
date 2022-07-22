#ifndef CodeGen_Function_h__
#define CodeGen_Function_h__

#include "CodeBlock.h"
#include "Variable.h"

namespace CodeGen {
  struct Function {
    String name;
    String returnType;
    Vector<Variable> params;
    CodeBlock contents;
    bool isConst;

    Function(
        String const& name,
        String const& returnType,
        CodeBlock const& contents = CodeBlock()) :
      name(name),
      returnType(returnType),
      contents(contents),
      isConst(false)
      {}

    Function(
        String const& name,
        String const& returnType,
        Vector<Variable> params,
        CodeBlock const& contents = CodeBlock()) :
      name(name),
      returnType(returnType),
      params(params),
      contents(contents),
      isConst(false)
      {}

    Function& Const() {
      isConst = true;
      return *this;
    }
  };

  CodeBlock& operator << (CodeBlock& block, Function const& f) {
    String prefix = Stringize()
      | f.returnType | " "
      | f.name;

    String qualifiers = f.isConst ? " const" : "";

    String suffix = f.contents.IsEmpty() ? ";" :
                    f.params.size() > 1 ? "" : " {";

    if (f.params.size() == 0) {
      block << prefix << "()" << qualifiers << suffix;
    }

    else if (f.params.size() == 1) {
      block << prefix << "("
            << f.params[0].type << " "
            << f.params[0].name
            << ")" << qualifiers << suffix;
    }

    else {
      block << prefix << "(";

      for (size_t i = 0; i < f.params.size(); ++i)
        block << kIndent
              << f.params[i].type << " "
              << f.params[i].name
              << (i + 1 < f.params.size() ? "," : ")")
              << (i + 1 < f.params.size() ? "" : qualifiers)
              << (i + 1 < f.params.size() ? "" : suffix);
    }

    if (!f.contents.IsEmpty()) {
      CodeBlock thisBlock(f.contents);
      thisBlock.Indent();

      if (f.params.size() > 1)
        block << "{";

      block << thisBlock << "}";
    }

    block.NewLine();
    return block;
  }
}

#endif
