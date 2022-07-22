#include "Static.h"
#include "Map.h"
#include "String.h"

typedef Map<String, Data> StaticMap;

namespace {
  StaticMap& GetMap() {
    static StaticMap map;
    return map;
  }
}

Data& Static_Get(String const& name) {
  return GetMap()[name];
}
