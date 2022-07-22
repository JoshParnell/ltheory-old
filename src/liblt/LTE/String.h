#ifndef LTE_String_h__
#define LTE_String_h__

#include "Common.h"
#include <cstring>
#include <string>
#include <sstream>

namespace LTE {
  struct String : public std::string {
    typedef NoBase BaseType;
    String() {}

    String(std::string const& str) :
      std::string(str)
      {}

    String(char const* str) :
      std::string(str)
      {}

    operator char const* () const {
      return c_str();
    }

    char& operator[](size_t index) {
      return (*((std::string*)this))[index];
    }

    char const& operator[](size_t index) const {
      return (*((std::string const*)this))[index];
    }

    char& back() {
      return (*this)[this->size() - 1];
    }

    char const& back() const {
      return (*this)[this->size() - 1];
    }

    bool contains(char c) const {
      for (size_t i = 0; i < size(); ++i)
        if ((*this)[i] == c)
          return true;
      return false;
    }

    bool contains(std::string const& str) const {
      return strstr(c_str(), str.c_str()) != nullptr;
    }

    bool containsOnly(String const& chars) const {
      for (size_t i = 0; i < size(); ++i)
        if (!chars.contains((*this)[i]))
          return false;
      return true;
    }

    char& front() {
      return (*this)[(size_t)0];
    }

    char const& front() const {
      return (*this)[(size_t)0];
    }

    void pop() {
      erase(end() - 1);
    }

    template <class StreamT>
    friend void _ToStream(StreamT& s, String const& t) {
      s << (std::string const&)t;
    }

    LT_API static void MapFields(TypeT*, void*, FieldMapper&, void*);
    DeclareMetadata(String)
  };

  template <class T>
  String ToString(T const& elem) {
    std::stringstream stream;
    stream.precision(4);
    ToStream(stream, elem);
    return stream.str();
  }

  template <>
  inline String ToString(String const& elem) {
    return elem;
  }

  template <class T>
  T FromString(String const& str) {
    T t;
    std::stringstream stream(str);
    stream >> t;
    return t;
  }

  template <>
  inline String FromString(String const& str) {
    return str;
  }

  inline String FormatMemoryString(size_t memory) {
    if (memory > 1024 * 1024)
      return ToString((float)memory / (1024 * 1024)) + "MB";
    else if (memory > 1024)
      return ToString((float)memory / 1024) + "KB";
    else
      return ToString(memory) + "B";
  }

  inline String String_Capital(String const& source) {
    String result;
    String buffer;
    std::stringstream stream(source);
    while (getline(stream, buffer, ' ')) {
      if (result.size())
        result += ' ';
      if (buffer.size())
        buffer.front() = (char)toupper(buffer.front());
      result += buffer;
    }
    return result;
  }

  inline bool String_Contains(String const& str, char c) {
    for (size_t i = 0; i < str.size(); ++i)
      if (str[i] == c)
        return true;
    return false;
  }

  inline size_t String_Distance(String const& s1, String const& s2) {
    size_t m = s1.size();
    size_t n = s2.size();
   
    if (m == 0) return n;
    if (n == 0) return m;
   
    size_t *costs = new size_t[n + 1];
   
    for (size_t i = 0; i <= n; i++)
      costs[i] = i;
   
    for (size_t i = 0; i < s1.size(); ++i) {
      costs[0] = i + 1;
      size_t corner = i;
   
      for (size_t j = 0; j < s2.size(); ++j) {
        size_t upper = costs[j + 1];
        if (s1[i] == s2[j])
          costs[j + 1] = corner;
        else {
          size_t t = upper < corner ? upper : corner;
          costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
        }
        corner = upper;
      }
    }
   
    size_t result = costs[n];
    delete[] costs;
    return result;
  }

  LT_API HashT String_Hash(String const&);

  struct String_Hasher {
    HashT operator()(String const& self) const {
      return String_Hash(self);
    }
  };

  LT_API bool String_IsAlpha(String const& self);
  LT_API bool String_IsNumeric(String const& self);

  inline String String_Lower(String const& self) {
    String result = self;
    for (size_t i = 0; i < self.size(); ++i)
      result[i] = (char)tolower(result[i]);
    return result;
  }

  inline String String_Replace(String const& self, char find, char replace) {
    String result = self;
    for (size_t i = 0; i < result.size(); ++i)
      if (result[i] == find)
        result[i] = replace;
    return result;
  }

  LT_API void String_Split(
    Vector<String>& result,
    String const& self,
    char delim);

  inline String String_Upper(String const& self) {
    String result = self;
    for (size_t i = 0; i < result.size(); ++i)
      result[i] = (char)toupper(result[i]);
    return result;
  }
}

struct Stringize {
  String string;

  Stringize() {}

  template <class T>
  Stringize(T const& t) {
    ToStream(*this, t);
  }

  operator LTE::String const&() {
    return string;
  }

  template <class T>
  Stringize& operator| (T const& t) {
    ToStream(*this, t);
    return *this;
  }

  template <class T>
  Stringize& operator<< (T const& t) {
    static std::stringstream stream;
    stream.str("");
    stream << t;
    string += stream.str();
    return *this;
  }
};

#endif
