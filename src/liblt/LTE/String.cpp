#include "String.h"
#include "Hash.h"
#include "Type.h"
#include "Vector.h"

namespace LTE {
  void String::MapFields(TypeT* type, void* addr, FieldMapper& m, void* aux) {
    String* self = (String*)addr;
    if (self) {
      size_t oldSize = self->size();
      size_t sz = oldSize;

      m(&sz, "size", Type_Get(sz), aux);

      if (sz != oldSize)
        self->resize(sz, ' ');

      for (size_t i = 0; i < sz; ++i)
        m(&(*self)[i], "data", Type_Get((*self)[i]), aux);
    }
  }

  DefineMetadata(String)

  HashT String_Hash(String const& source) {
    return Hash(source.data(), source.size());
  }

  bool String_IsAlpha(String const& self) {
    const String alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return self.containsOnly(alpha);
  }

  bool String_IsNumeric(String const& self) {
    const String numeric = "+-0123456789.";
    return self.containsOnly(numeric);
  }

  void String_Split(
    Vector<String>& result,
    String const& self,
    char delim)
  {
    std::stringstream stream(self);
    String buf;
    result.clear();
    while (getline(stream, buf, delim))
      result.push(buf);
  }
}
