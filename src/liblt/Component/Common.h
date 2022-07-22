#include "LTE/Common.h"
#include "LTE/Pointer.h"
#include "Game/Common.h"

#define AutoComponent(x)                                                       \
  template <class BaseT>                                                       \
  struct Component_##x : public BaseT {                                        \
    Component##x x;                                                            \
                                                                               \
    Pointer<Component##x> Get##x() { return &x; }                              \
                                                                               \
    FIELDS {                                                                   \
      Component_##x* self = (Component_##x*)addr;                              \
      m(&self->x, #x, Type_Get(self->x), aux);                                 \
      BaseT::MapFields(type, addr, m, aux);                                    \
    }
