#include "LTSL.h"
#include "StringList.h"

namespace {
  void RewriteDot(StringList& list) {
    String const& value = list->GetValue();
    if (!String_IsNumeric(value) &&
         value.contains('.') &&
        !value.contains('"'))
    {
      Vector<String> parts;
      String_Split(parts, value, '.');
      StringList newList = new StringListAtom(parts[0]);
      for (size_t i = 1; i < parts.size(); ++i)
        newList = new StringListList(Vector<StringList>(
          new StringListAtom(parts[i]),
          newList));

      list = newList;
    }
  }

  bool IsBinaryOp(StringList const& list, Vector<String> const& ops) {
    String const& value = list->GetValue();
    for (size_t i = 0; i < ops.size(); ++i)
      if (value == ops[i])
        return true;
    return false;
  }

  void RewriteBinaryOp(StringList& list, Vector<String> const& ops) {
    StringListList* l = (StringListList*)list.t;
    for (int i = 0; i + 2 < (int)l->elements.size(); ++i) {
      if (IsBinaryOp(l->elements[i + 1], ops)) {
        l->elements[i] = new StringListList(Vector<StringList>(
          l->elements[i + 1],
          l->elements[i],
          l->elements[i + 2]));
        l->elements.eraseIndex(i + 1);
        l->elements.eraseIndex(i + 1);
        i--;
      }
    }
  }

  void RewriteAtom(StringList& list) {
    RewriteDot(list);
  }

  void RewriteList(StringList& list) {
    static Vector<String> precedence[] = {
      Vector<String>() << "^",
      Vector<String>() << "*" << "/",
      Vector<String>() << "+" << "-",
      Vector<String>() << "<" << ">" << "<=" << ">=",
      Vector<String>() << "==" << "!=",
      Vector<String>() << "&&",
      Vector<String>() << "||",
      Vector<String>() << "=" << "+=" << "-=" << "*=" << "/="
    };

    for (uint i = 0; i < sizeof(precedence) / sizeof(*precedence); ++i)
      RewriteBinaryOp(list, precedence[i]);
  }

  void Rewrite(StringList& list) {
    StringListList* l = (StringListList*)list.t;
    for (size_t i = 0; i < l->elements.size(); ++i) {
      if (l->elements[i]->IsAtom())
        RewriteAtom(l->elements[i]);
      else
        Rewrite(l->elements[i]);
    }
    RewriteList(list);
  }
}

StringList LTSL_ApplyRewrites(StringList const& list) {
  StringList newList = list;
  Rewrite(newList);
  return newList;
}
