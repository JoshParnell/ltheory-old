#ifndef CodeGen_Class_h__
#define CodeGen_Class_h__

#include "CodeBlock.h"

namespace CodeGen {
  struct Class {
    String const& name;
    Vector<String> bases;
    CodeBlock contents;

    Class(
        String const& name,
        CodeBlock const& contents) :
      name(name),
      contents(contents)
      {}

    Class(
        String const& name,
        String const& singleBase,
        CodeBlock const& contents) :
      name(name),
      contents(contents)
    {
      bases.push(singleBase);
    }
  };

  CodeBlock& operator << (CodeBlock& block, Class const& c) {
    CodeBlock contents = c.contents;
    contents.Indent();

    if (c.bases.size())
      block << "struct " << c.name << " : public " << c.bases[0] << " {";
    else
      block << "struct " << c.name << " {";

    block << contents;
    block << "}; /* " << c.name << ". */";
    block.NewLine();
    return block;
  }
}

#endif
