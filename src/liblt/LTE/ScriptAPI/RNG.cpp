#include "LTE/Function.h"
#include "LTE/RNG.h"

TypeAlias(Reference<RNGT>, RNG);

FreeFunction(RNG, RNG_MTG,
  "Create a new RNG that uses the Mersenne Twister algorithm, seeded with 'seed'",
  int, seed)
{
  return RNG_MTG((uint)seed);
}

FreeFunction(float, RNG_Angle,
  "Generate a uniform random angle between 0 and 2pi using 'rng'",
  RNG, rng)
{
  return rng->GetAngle();
} FunctionAlias(RNG_Angle, Angle);

FreeFunction(V3, RNG_Direction,
  "Generate a uniform random (unit-length) vector using 'rng'",
  RNG, rng)
{
  return rng->GetDirection();
} FunctionAlias(RNG_Direction, Direction);

FreeFunction(float, RNG_ExpFloat,
  "Generate an exponential random float (with mean 1) using 'rng'",
  RNG, rng)
{
  return rng->GetExp();
} FunctionAlias(RNG_ExpFloat, Exp);

namespace Priv1 {
  FreeFunction(float, RNG_Float,
    "Generate a uniform random float between 0 (inclusive) and 1 (exclusive) using 'rng'",
    RNG, rng)
  {
    return rng->GetFloat();
  }
}

namespace Priv2 {
  FreeFunction(float, RNG_Float,
    "Generate a uniform random float between 'lower' (inclusive) and 'upper' (exclusive) using 'rng'",
    RNG, rng,
    float, lower,
    float, upper)
  {
    return rng->GetFloat(lower, upper);
  }
} 

FunctionAlias(RNG_Float, Float);

namespace Priv1 {
  FreeFunction(float, RNG_Gaussian,
    "Generate a gaussian random float with mean 0 and variance 1 using 'rng'",
    RNG, rng)
  {
    return rng->GetGaussian();
  }
}

namespace Priv2 {
  FreeFunction(float, RNG_Gaussian,
    "Generate a gaussian random float with mean 0 and variance 'variance' using 'rng'",
    RNG, rng,
    float, variance)
  {
    return variance * rng->GetGaussian();
  }
}

FunctionAlias(RNG_Gaussian, Gaussian);

namespace Priv1 {
  FreeFunction(int, RNG_Int,
    "Generate a uniform random int between 'lower' and 'upper' (inclusive) using 'rng'",
    RNG, rng,
    int, lower,
    int, upper)
  {
    return rng->GetInt(lower, upper);
  }
}

namespace Priv2 {
  FreeFunction(int, RNG_Int,
    "Generate a uniform random int between 0 (inclusive) and 'upper' (exclusive) using 'rng'",
    RNG, rng,
    int, upper)
  {
    return rng->GetInt(0, upper - 1);
  }
}

namespace Priv3 {
  FreeFunction(int, RNG_Int,
    "Generate a uniform random int over all possible integer values using 'rng'",
    RNG, rng)
  {
    return rng->GetInt();
  }
}

FunctionAlias(RNG_Int, Int);

FreeFunction(V3, RNG_Sign,
  "Generate -1 or 1 with equal probability using 'rng'",
  RNG, rng)
{
  return rng->GetFloat() < 0.5f ? -1.0f : 1.0f;
} FunctionAlias(RNG_Sign, Sign);

FreeFunction(V3, RNG_Sphere,
  "Generate a uniform random vector that lies within the unit sphere",
  RNG, rng)
{
  return rng->GetSphere();
} FunctionAlias(RNG_Sphere, Sphere);

FreeFunction(V2, RNG_Vec2,
  "Generate a uniform random Vec2 with components between 'lower' (inclusive) and 'upper' (exclusive) using 'rng'",
  RNG, rng,
  V2, lower,
  V2, upper)
{
  return rng->GetV2(lower, upper);
} FunctionAlias(RNG_Vec2, Vec2);

FreeFunction(V3, RNG_Vec3,
  "Generate a uniform random Vec3 with components between 'lower' (inclusive) and 'upper' (exclusive) using 'rng'",
  RNG, rng,
  V3, lower,
  V3, upper)
{
  return rng->GetV3(lower, upper);
} FunctionAlias(RNG_Vec3, Vec3);
