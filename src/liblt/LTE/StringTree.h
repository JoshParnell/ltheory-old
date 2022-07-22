#ifndef LTE_StringTree_h__
#define LTE_StringTree_h__

#include "AutoClass.h"
#include "Map.h"
#include "Pointer.h"
#include "Reference.h"
#include "String.h"
#include "Vector.h"

AutoClassDerived(StringTreeT, RefCounted,
  String, value,
  int, line,
  Vector<StringTree>, children)

  StringTreeT() : line(-1) {}

  StringTree const& GetChild(size_t index) const {
    return children[index];
  }

  size_t GetChildren() const {
    return children.size();
  }

  template <class T>
  bool ToType(T& t) const {
    std::stringstream stream(value);
    stream >> t;
    return !stream.bad();
  }

  bool ToBool() const {
    return String_Lower(value) == "true" ? true : false;
  }

  double ToDouble() const {
    double d;
    return ToType<double>(d) ? d : 0.0;
  }

  float ToFloat() const {
    float f;
    return ToType<float>(f) ? f : 0.0f;
  }

  int ToInt() const {
    int i;
    return ToType<int>(i) ? i : 0;
  }

  void Print(std::ostream& s, int indentLevel = 0) const {
    for (int i = 0; i < indentLevel; ++i)
      ToStream(s, "  ");

    if (value.size())
      ToStream(s, value);
    else
      ToStream(s, "(null)");

    for (size_t i = 0; i < children.size(); ++i) {
      ToStream(s, '\n');
      children[i]->Print(s, indentLevel + 1);
    }
  }
};

LT_API StringTree StringTree_Create(String const& data, int& line);

inline StringTree StringTree_Create(String const& data) {
  int line = 0;
  return StringTree_Create(data, line);
}

LT_API StringTree StringTree_Load(Location const& location);

#endif
