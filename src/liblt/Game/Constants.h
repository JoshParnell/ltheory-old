#ifndef Game_Constants_h__
#define Game_Constants_h__

#include "Common.h"

/* Fundamental Units of Gameplay
 *  DU - Distance Unit    -> 1 km
 *  MU - Mass Unit        -> 1 kg
 *  RU - Resource Unit    -> 1 J
 *  TU - Time unit        -> 1 S
 *  OU - Output unit      -> RU / TU          -> 1 W
 *  XU - Transfer Unit    -> (RU * DU) / TU
 */

/* Balancing is performed in terms of abstract resource units, 'RU.'
 * RU can be thought of as energy, and, throughout the game, we will strive to
 * balance the creation and destruction of RU in much the same way that the
 * real world conserves energy.  To do this, we will need to define
 * relationships that allow RU to be converted between different forms. */

/* We define the 'power' or 'output' of an object as the rate at which the
 * object can bring RU into existence.  Hence, 1 'output unit' or OU, is
 * equivalent to the ability to create one RU every second, or, in if the
 * process requires input, to convert the input at a rate of 1 RU per second. */

/* How much RU is contained in the assemblyChip of a 1 RU object/item? */
inline double Constant_AssemblyChipRatio() {
  return 10.0;
}

/* How much RU should the next research tier be worth if the current is
 * worth x RU?
 *
 *  x' = k * x          -> x(t) ~ e^kt
 *  x' = k * sqrt(x)    -> x(t) ~ t^2
 *  x' = k              -> x(t) ~ t
 *  x' = e^-kx          -> x(t) ~ log(t) */
inline double Constant_ResearchIncrement(double x) {
  return x + 0.25 * Sqrt(x);
}

/* How much RU is contained in 1 OU that creates RU? */
inline double Constant_CreationRatio() {
  return 50.0;
}

/* How much RU is contained in 1 OU that destroys RU? */
inline double Constant_DestructionRatio() {
  return 10.0;
}

/* VALUE CONVERSION. */

  /* Value -> Mass. */
  inline Mass Constant_ValueToMass(double x, float compactness = 1.0f) {
    return (Mass)((x / compactness) / 10.0f);
  }

  /* Value -> Capacity. */
  inline Mass Constant_ValueToCapacity(double x, float capacity = 1.0f) {
    return (Mass)(Constant_ValueToMass(x * capacity) / 10.0f);
  }

  /* Value -> Integrity. */
  inline Health Constant_ValueToIntegrity(double x, float strength = 1.0f) {
    return (Health)(Constant_ValueToMass(x * strength) / 10.0f);
  }

  /* Value -> Output. */
  inline float Constant_ValueToOutput(double x, float rate = 1.0f) {
    return (float)(x * rate);
  }

  /* Value -> Power Drain. */
  inline float Constant_ValueToPowerDrain(double x, float efficiency = 1.0f) {
    return (float)(x / efficiency);
  }

/* SECONDARY CONVERSION. */

  /* Mass -> Scale. */
  /* NOTE : Realistically, this power should be somewhere between 1/3 and 1/2.
   *        1/3 assumes mostly solid-volume objects in the game, while 1/2
   *        assumes mostly thin-shell objects. */
  inline float Constant_MassToScale(Mass x) {
    return (float)(Pow(x / 1000.0f, 1.0f / 2.0f));
  }

/* RATIO. */

  inline float Constant_RangeRatio(float x) {
    return 1000.0f * x;
  }

  inline float Constant_SpeedRatio(float x) {
    return 1000.0f * Pow(x, 1.5f);
  }

/* BASELINE MULTIPLIERS. */

  inline float Constant_AmmoDamageMult() {
    return 2;
  }

  inline float Constant_AmmoLifeMult() {
    return 4.0f;
  }

  inline float Constant_AmmoSpeedMult() {
    return 500;
  }

  inline float Constant_WeaponRateMult() {
    return 0.10f;
  }

  inline float Constant_WeaponSpreadMult() {
    return 0.001f;
  }

#endif
