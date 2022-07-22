#ifndef LTE_Button_h__
#define LTE_Button_h__

#include "BaseType.h"
#include "Reference.h"

namespace LTE {
  struct ButtonT : public RefCounted {
    BASE_TYPE(ButtonT)

    virtual bool Down() const = 0;
    virtual float DownTime() const = 0;
    virtual bool Pressed() const = 0;
    virtual bool Released() const = 0;

    FIELDS {}
  };
}

#endif
