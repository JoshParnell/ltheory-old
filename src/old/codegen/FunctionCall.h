#ifndef CodeGen_FunctionCall_h__
#define CodeGen_FunctionCall_h__

#include "CodeBlock.h"
#include "Variable.h"

namespace CodeGen {
  struct FunctionCall {
    String function;
    Vector<String> args;
    bool isMacro;

    FunctionCall(String const& function) :
      function(function),
      isMacro(false)
      {}

    FunctionCall(
        String const& function,
        Vector<String> const& args) :
      function(function),
      args(args),
      isMacro(false)
      {}

    FunctionCall(
        String const& function,
        Vector<Variable> const& vars) :
      function(function)
    {
      for (size_t i = 0; i < vars.size(); ++i)
        args.push(vars[i].name);
    }

    FunctionCall& Macro() {
      isMacro = true;
      return *this;
    }
  };

  CodeBlock& operator << (CodeBlock& block, FunctionCall const& c) {
    block << c.function << (c.args.size() ? "(" : "()");
    if (c.args.size()) {
      for (size_t i = 0; i < c.args.size(); ++i) {
        block << (c.isMacro ? "" : kIndent) << c.args[i]
              << (i + 1 < c.args.size() ? "," : ")");
        if (c.isMacro)
          block.Join();
      }
    }

    if (!c.isMacro)
      block.lines.back() += ";";

    return block;
  }
}

#endif
