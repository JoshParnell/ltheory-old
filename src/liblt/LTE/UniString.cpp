#include "UniString.h"
#include "String.h"

#include "UTF8/unchecked.h"

void UniStringIterator::Advance() {
  codepoint = utf8::unchecked::next(iter);
}

UniStringIterator UniString_Begin(String const& source) {
  UniStringIterator it;
  it.iter = source.data();
  it.end = source.data() + source.size() + 1;
  it.codepoint = utf8::unchecked::next(it.iter);
  return it;
}
