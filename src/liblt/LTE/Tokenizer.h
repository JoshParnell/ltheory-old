#ifndef LTE_Tokenizer_h__
#define LTE_Tokenizer_h__

#include "String.h"
#include "Vector.h"

namespace LTE {
  struct Tokenizer {
    String data;
    size_t cursor;

    Tokenizer(String const& data) :
      data(data),
      cursor(0)
      {}

    void Advance() {
      cursor++;
    }

    bool HasMore() const {
      return cursor < data.size();
    }

    size_t GetCursor() const {
      return cursor;
    }

    char Peek() const {
      return data[cursor];
    }

    char Read() {
      return data[cursor++];
    }

    String ReadDelimited(String const& openDelim, String const& closeDelim) {
      String token;
      ReadDelimitedInPlace(token, openDelim, closeDelim);
      return token;
    }

    void ReadDelimitedInPlace(
      String& output,
      String const& openDelim,
      String const& closeDelim)
    {
      int level = 1;
      while (HasMore() && level) {
        char c = Read();
        if (openDelim.contains(c))
          level++;
        else if (closeDelim.contains(c))
          level--;

        if (level)
          output += c;
      }
    }

    String ReadIndented(uint baseLevel, String const& indent) {
      String result;
      while (HasMore()) {
        String line = ReadLine();
        uint level = GetIndent(line, indent);
        if (level <= baseLevel)
          break;

        if (result.size())
          result += "\n" + line;
        else
          result = line;
      }

      return result;
    }

    String ReadLine() {
      return ReadToken("\n", "\r");
    }

    String ReadSubset(String const& subset) {
      String result;
      while (HasMore() && subset.contains(Peek()))
        result += Read();
      return result;
    }

    String ReadToken(
      String const& delim,
      String const& ignore = "",
      String const& scopeOpen = "",
      String const& scopeClose = "")
    {
      String token;
      ReadTokenInPlace(
        token,
        delim,
        ignore,
        scopeOpen,
        scopeClose);
      return token;
    }

    void ReadTokenInPlace(
      String& output,
      String const& delim,
      String const& ignore = "",
      String const& scopeOpen = "",
      String const& scopeClose = "")
    {
      uint scope = 0;
      bool escaped = false;
      bool literal = false;
      bool emit = true;

      while (HasMore()) {
        char c = Read();

        if (c == '\"' && !escaped)
          literal = !literal;

        if (escaped)
          escaped = false;

        if (!literal) {
          if (ignore.contains(c)) {
            cursor++;
            continue;
          }

          if (scopeOpen.contains(c)) {
            emit = scope > 0;
            scope++;
          } else if (scopeClose.contains(c)) {
            scope--;
            emit = scope > 0;
          } else if (scope == 0 && delim.contains(c))
            break;
        } else {
          if (c == '\\')
            escaped = true;
        }

        if (emit)
          output += c;
        else
          emit = true;
      }
    }

    void SetCursor(size_t cursor) {
      this->cursor = cursor;
    }

    void Split(
      Vector<String>& output,
      String const& delim,
      String const& excludeOpen = "",
      String const& excludeClose = "")
    {
      while (HasMore()) {
        String token;
        ReadTokenInPlace(token, delim, "", excludeOpen, excludeClose);
        if (token.size())
          output.push(token);
      }
    }

    static uint GetIndent(String const& line, String const& indent) {
      uint level = 0;
      while (level < line.size() && indent.contains(line[level]))
        level++;
      return level;
    }
  };
}

#endif
