#ifndef Game_Signature_h__
#define Game_Signature_h__

#include "Common.h"
#include "LTE/AutoClass.h"

/* NOTE : Frequency is expressed in log hz.
 *        3  -> 1 KHz (Sound)
 *        6  -> 1 MHz (Radio)
 *        9  -> 1 GHz (Cell / Microwave / Wireless)
 *        12 -> 1 THz (Visible)
 *        15 -> 1 PHz (UV)
 *        18 -> 1 EHz (X-ray)
 *        21 -> 1 ZHz (Gamma) */

AutoClass(Signature,
  float, amplitude,
  float, frequency,
  float, variance,
  float, power)

  Signature() {}
};

#endif
