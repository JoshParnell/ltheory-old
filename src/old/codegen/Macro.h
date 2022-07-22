#ifndef CodeGen_Macro_h__
#define CodeGen_Macro_h__

#include "CodeBlock.h"

namespace CodeGen {
  struct Macro {
    String name;
    Vector<String> params;
    CodeBlock contents;

    Macro(
        String const& name,
        Vector<String> const& params,
        CodeBlock const& contents) :
      name(name),
      params(params),
      contents(contents)
      {}
  };

  CodeBlock& operator << (CodeBlock& block, Macro const& m) {
    Stringize str;
    str | "#define " | m.name | "(";
    for (size_t i = 0; i < m.params.size(); ++i) {
      if (i)
        str | ", ";
      str | m.params[i];
    }
    str | ") \\";

    block << (String)str;

    CodeBlock thisBlock(m.contents);
    thisBlock.Indent();
    for (size_t i = 0; i < thisBlock.lines.size(); ++i) {
      while (thisBlock.lines[i].size() < 79)
        thisBlock.lines[i] += ' ';
      thisBlock.lines[i] += '\\';
    }

    block << thisBlock;
    block.NewLine();
    return block;
  }
}

#endif
