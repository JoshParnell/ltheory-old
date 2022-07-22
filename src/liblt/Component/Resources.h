#ifndef Component_Resources_h__
#define Component_Resources_h__

#include "Common.h"
#include "LTE/AutoClass.h"
#include "LTE/Distribution.h"

AutoClass(ComponentResources,
  Distribution<Item>, elements)

  ComponentResources() {}
};

AutoComponent(Resources)
};

#endif
