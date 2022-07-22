#ifndef LTE_StringList_h__
#define LTE_StringList_h__

#include "AutoClass.h"
#include "BaseType.h"
#include "Pool.h"
#include "Reference.h"
#include "Vector.h"

struct StringListT : public RefCounted {
  BASE_TYPE(StringListT)

  virtual StringList Clone() const = 0;

  virtual StringList Get(size_t index) const {
    return nullptr;
  }

  virtual size_t GetSize() const {
    return 0;
  }

  virtual String GetString() const = 0;

  virtual String GetValue() const {
    return "";
  }

  virtual bool IsAtom() const = 0;
};

AutoClassDerived(StringListAtom, StringListT,
  String, value)
  DERIVED_TYPE_EX(StringListAtom)
  POOLED_TYPE

  StringListAtom() {}

  StringList Clone() const {
    return new StringListAtom(*this);
  }

  size_t GetSize() const {
    return 1;
  }

  String GetString() const {
    return value;
  }

  String GetValue() const {
    return value;
  }

  bool IsAtom() const {
    return true;
  }
};

AutoClassDerived(StringListList, StringListT,
  Vector<StringList>, elements)
  DERIVED_TYPE_EX(StringListList)
  POOLED_TYPE
  
  StringListList() {}

  StringList Clone() const {
    return new StringListList(*this);
  }

  StringList Get(size_t index) const {
    return elements[index];
  }

  size_t GetSize() const {
    return elements.size();
  }

  String GetString() const {
    String str = "(";
    for (size_t i = 0; i < elements.size(); ++i) {
      if (i)
        str += " ";
      str += elements[i]->GetString();
    }

    return str + ")";
  }
  
  bool IsAtom() const {
    return false;
  }
};

LT_API StringList StringList_Create(String const& data);
LT_API StringList StringList_Load(Location const& location);
LT_API void StringList_Print(StringList const& list);

#endif
