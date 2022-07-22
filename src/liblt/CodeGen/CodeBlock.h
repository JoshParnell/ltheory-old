#ifndef CodeGen_CodeBlock_h__
#define CodeGen_CodeBlock_h__

#include "Common.h"
#include "LTE/String.h"
#include "LTE/Vector.h"

#include <fstream>

namespace CodeGen {
  struct NewLine {};

  char const* kConstString = "char const*";
  char const* kIndent = "  ";

  struct CodeBlockT : public RefCounted {
    Vector<String> lines;

    CodeBlockT() {}

    CodeBlockT(String const& line) {
      lines.push(line);
    }

    void AddLine(String const& line) {
      lines.push(line);
    }

    CodeBlockT Indent() {
      for (size_t i = 0; i < lines.size(); ++i)
        lines[i] = kIndent + lines[i];
      return this;
    }

    bool IsEmpty() const {
      return lines.isEmpty();
    }

    CodeBlock Join() {
      if (lines.size() < 2)
        return this;

      lines[lines.size() - 2] += lines.back();
      lines.pop();
      return this;
    }

    CodeBlock NewLine() {
      lines.push("");
      return this;
    }

    void SaveTo(String const& file) {
      std::ofstream stream(file.c_str());
      for (size_t i = 0; i < lines.size(); ++i)
        stream << lines[i] << std::endl;
    }
  };

  struct AutoNewline {
    CodeBlock block;
    String contents;

    AutoNewline(CodeBlock block) :
      block(block)
      {}

    ~AutoNewline() {
      if (contents.size())
        block->lines.push(contents);
    }

    template <class T>
    AutoNewline& operator << (T const& t) {
      contents += Stringize() | t;
      return *this;
    }
  };

  CodeBlock operator << (CodeBlock const& block, CodeBlock const& other) {
    for (size_t i = 0; i < other->lines.size(); ++i)
      block->lines.push(other->lines[i]);
    return block;
  }

  template <class T>
  AutoNewline operator << (CodeBlock const& block, T const& t) {
    AutoNewline nl(block);
    nl << t;
    return nl;
  }

  inline CodeBlock CodeBlock_Create() {
    return new CodeBlockT;
  }
}

#endif
