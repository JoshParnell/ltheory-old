#ifndef LTE_Vertex_h__
#define LTE_Vertex_h__

#include "AutoClass.h"
#include "V2.h"
#include "V3.h"

AutoClass(Vertex,
  V3, p,
  V3, n,
  float, u,
  float, v)

  Vertex() {}

  V2 UV() const {
    return V2(u, v);
  }
};

#endif
