#include "Icon.h"

DefineFunction(Icon_Create) {
  return new IconT;
} FunctionAlias(Icon_Create, Icon);
