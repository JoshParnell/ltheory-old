#ifndef LTE_LocationT_h__
#define LTE_LocationT_h__

#include "BaseType.h"
#include "DeclareFunction.h"
#include "Reference.h"
#include "String.h"

namespace LTE {
  struct LocationT : public RefCounted {
    BASE_TYPE(LocationT)

    LT_API HashT GetHash() const;
    LT_API String ReadAscii() const;

    virtual Location Clone() const = 0;
    virtual bool Exists() const = 0;
    virtual AutoPtr< Array<uchar> > Read() const = 0;
    virtual bool Write(Array<uchar> const& data) const = 0;

    FIELDS {}
  };

  DeclareFunction(Location_Cache, Location,
    String, name)

  DeclareFunction(Location_File, Location,
    String, file)

  LT_API Location Location_Memory(String const& str);

  LT_API Location Location_Memory(Array<uchar>* memory, bool ownsMemory = false);

  DeclareFunction(Location_Resource, Location,
    String, name)

  DeclareFunction(Location_Web, Location,
    String, host,
    String, file)

  inline Location Location_Font(String const& name) {
    return Location_Resource("font/" + name);
  }

  inline Location Location_GameData(String const& name) {
    return Location_Resource("gamedata/" + name);
  }

  inline Location Location_Script(String const& name) {
    return Location_Resource("script/" + name);
  }

  inline Location Location_Shader(String const& name) {
    return Location_Resource("shader/" + name);
  }

  inline Location Location_Texture(String const& name) {
    return Location_Resource("texture/" + name);
  }
}

#endif
