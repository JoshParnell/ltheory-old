#ifndef Attribute_Mass_h__
#define Attribute_Mass_h__

#include "Common.h"

/* NOTE : Measured in metric tons (t)
 *
 * Reference:
 *
 *    Sun                       ~ 2e27
 *    Earth                     ~ 6e21
 *    Moon                      ~ 7e19
 *    Aircraft Carrier          ~ 100,000
 *    Battlecruiser             ~  35,000
 *    Battleship                ~  30,000
 *    Cruiser                   ~  10,000
 *    Destroyer                 ~   8,000
 *    Frigate                   ~   4,000
 *    Gunboat                   ~   1,500
 *    Space Shuttle             ~   2,000
 *    Space Station             ~     400
 *    F-16 (Loaded)             ~      12
 *    Human Body                ~       0.07
 *    M249                      ~       0.007
 */

template <class T>
struct Attribute_Mass : public T {
  typedef Attribute_Mass SelfType;
  ATTRIBUTE_COMMON(mass)
  Mass mass;

  Attribute_Mass() :
    mass(0)
    {}

  Mass const& GetMass() const {
    return mass;
  }

  bool HasMass() const {
    return true;
  }
};

#endif
