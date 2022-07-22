#ifndef Attribute_Density_h__
#define Attribute_Density_h__

#include "Common.h"

/* NOTE : Measured in metric tons / cubic meter.
 *
 * Reference:
 *
 *    Aluminium                 ~ 3
 *    Carbon                    ~ 2
 *    Copper                    ~ 9
 *    Crops                     ~ 0.5 - 1.0
 *    Dirt                      ~ 1.5 - 2.0
 *    Gold                      ~ 19
 *    Ice                       ~ 1
 *    Iron                      ~ 8
 *    Nickel                    ~ 9
 *    Platinum                  ~ 21
 *    Rock                      ~ 2.0 - 3.0
 *    Silver                    ~ 10
 *    Steel                     ~ 8
 *    Tin                       ~ 7
 *    Uranium                   ~ 19
 *    Water                     = 1
 *    Wood                      ~ 0.5 - 1.0
 */

template <class T>
struct Attribute_Density : public T {
  typedef Attribute_Density SelfType;
  ATTRIBUTE_COMMON(density)
  float density;

  Attribute_Density() :
    density(1)
    {}

  float GetDensity() const {
    return density;
  }

  bool HasDensity() const {
    return true;
  }
};

#endif
