#include "LTE/Function.h"
#include "LTE/String.h"
#include "LTE/Vector.h"

DefineConversion(bool_to_string, bool, String) {
  dest = src ? "true" : "false";
}

DefineConversion(float_to_string, float, String) { dest = ToString<float>(src); }
DefineConversion(double_to_string, double, String) { dest = ToString<double>(src); }
DefineConversion(schar_to_string, signed char, String) { dest = ToString<signed char>(src); }
DefineConversion(uchar_to_string, unsigned char, String) { dest = ToString<unsigned char>(src); }
DefineConversion(sshort_to_string, signed short, String) { dest = ToString<signed short>(src); }
DefineConversion(ushort_to_string, unsigned short, String) { dest = ToString<unsigned short>(src); }
DefineConversion(sint_to_string, signed int, String) { dest = ToString<signed int>(src); }
DefineConversion(uint_to_string, unsigned int, String) { dest = ToString<unsigned int>(src); }
DefineConversion(slong_to_string, signed long, String) { dest = ToString<signed long>(src); }
DefineConversion(ulong_to_string, unsigned long, String) { dest = ToString<unsigned long>(src); }
DefineConversion(sllong_to_string, signed long long, String) { dest = ToString<signed long long>(src); }
DefineConversion(ullong_to_string, unsigned long long, String) { dest = ToString<unsigned long long>(src); }
DefineConversion(int32_to_string, int32, String) { dest = ToString<int32>(src); }
DefineConversion(int64_to_string, int64, String) { dest = ToString<int64>(src); }
DefineConversion(uint32_to_string, int32, String) { dest = ToString<uint32>(src); }
DefineConversion(uint64_to_string, int64, String) { dest = ToString<uint64>(src); }

VoidFreeFunction(String_Append,
  "Append 'b' to 'a'",
  String, a,
  String, b)
{
  (String&)a += b;
} FunctionAlias(String_Append, +=);

FreeFunction(String, String_CapitalCase,
  "Return a new version of 's' where each word is capitalized",
  String, s)
{
  return String_Capital(s);
} FunctionAlias(String_CapitalCase, CapitalCase);

FreeFunction(String, String_Concat,
  "Return the concatenation of the strings 'a' and 'b'",
  String, a,
  String, b)
{
  return a + b;
} FunctionAlias(String_Concat, +);

FreeFunction(bool, String_Contains,
  "Return whether 's' contains 'substring'",
  String, s,
  String, substring)
{
  if (substring.size() == 0)
    return true;
  return s.contains(substring);
} FunctionAlias(String_Contains, Contains);

FreeFunction(bool, String_IsEmpty,
  "Return whether 's' is of zero length",
  String, s)
{
  return s.size() == 0;
} FunctionAlias(String_IsEmpty, IsEmpty);

FreeFunction(bool, String_Equal,
  "Return a == b",
  String, a,
  String, b)
{
  return a == b;
} FunctionAlias(String_Equal, ==);

FreeFunction(char, String_Get,
  "Return the 'i'th character in 's'",
  String, s,
  int, i)
{
  return s[size_t(i)];
} FunctionAlias(String_Get, Get);

FreeFunction(HashT, String_GetHash,
  "Return a hash for 's'",
  String, s)
{
  return String_Hash(s);
} FunctionAlias(String_GetHash, GetHash);

FreeFunction(int, String_Length,
  "Return the number of characters in 's'",
  String, s)
{
  return s.size();
} FunctionAlias(String_Length, Length);

FreeFunction(String, String_LowerCase,
  "Return a new version of 's' where each letter is lower case",
  String, s)
{
  return String_Lower(s);
} FunctionAlias(String_LowerCase, LowerCase);

FreeFunction(bool, String_NotEqual,
  "Return a != b",
  String, a,
  String, b)
{
  return a != b;
} FunctionAlias(String_NotEqual, !=);

VoidFreeFunction(String_Pop,
  "Delete the last character of 's'",
  String, s)
{
  Mutable(s).pop();
} FunctionAlias(String_Pop, Pop);

FreeFunction(Vector<String>, String_Split,
  "Split 's' into pieces using 'delimiter'",
  String, s,
  char, delimiter)
{
  Vector<String> result;
  String_Split(result, s, delimiter);
  return result;
} FunctionAlias(String_Split, Split);

FreeFunction(Vector<String>, String_SplitLines,
  "Split 's' into pieces using newline as a delimiter",
  String, s)
{
  Vector<String> result;
  String_Split(result, s, '\n');
  return result;
} FunctionAlias(String_SplitLines, SplitLines);

FreeFunction(String, String_Substring,
  "Return the substring of 'length' from 's' starting at 'start'",
  String, s,
  int, start,
  int, length)
{
  return s.substr(0, length);
} FunctionAlias(String_Substring, Substring);

FreeFunction(float, String_ToFloat,
  "Return 's' converted to a float",
  String, s)
{
  return FromString<float>(s);
} FunctionAlias(String_ToFloat, ToFloat);

FreeFunction(int, String_ToInt,
  "Return 's' converted to an int",
  String, s)
{
  return FromString<int>(s);
} FunctionAlias(String_ToInt, ToInt);

FreeFunction(String, String_UpperCase,
  "Return a new version of 's' where each letter is upper case",
  String, s)
{
  return String_Upper(s);
} FunctionAlias(String_UpperCase, UpperCase);
