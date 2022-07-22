#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Info.h"
#include "Component/Interior.h"
#include "Component/Pluggable.h"
#include "Component/Supertyped.h"

#include "Game/Player.h"

#include "LTE/Math.h"

const int kSamples = 1024;

typedef ObjectWrapper
  < Component_Pluggable
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_Scanner>
  > > >
  ScannerBaseT;

AutoClassDerivedEmpty(Scanner, ScannerBaseT)
  InteriorIterator iterator;

  DERIVED_TYPE_EX(Scanner)
  POOLED_TYPE

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    /* CRITICAL : Disabled scanners. */
    return;

    Pluggable.powerRequest = Supertyped.type->GetPowerDrain();
    float rangeSquared = Supertyped.type->GetRange() * GetPowerFraction();

    rangeSquared *= rangeSquared;
    Player const& player = parent->GetOwner();
    if (!player)
      return;

    Pointer<ComponentInfo> info = player->GetInfo();
    if (info) {
      Position myPos = parent->GetPos();
      for (int i = 0; i < kSamples; ++i) {
        if (!iterator.HasMore()) {
          iterator = Object_GetInteriorObjects(GetContainer().t);
          if (!iterator.HasMore())
            break;
        }

        Object const& current = iterator.Get();
        Pointer<ComponentBoundingBox> bb = current->GetBoundingBox();
        if (bb) {
          Distance factor = current->GetGlobalBound().GetSurfaceArea();
          Distance distance = LengthSquared(myPos - current->GetPos()) / factor;
          if (distance <= rangeSquared)
            info->Add(current, InfoLevel_Location);
        }

        iterator.Advance();
      }
    }
  }
};

DERIVED_IMPLEMENT(Scanner)

DefineFunction(Object_Scanner) {
  LTE_ASSERT(args.type->GetType() == ItemType_ScannerType);
  Reference<Scanner> self = new Scanner;
  self->SetSupertype(args.type);
  return self;
}

AutoClass(ScannerOutput,
  Array<float>, bands)
  ScannerOutput() {}

  ScannerOutput(size_t size) {
    bands.resize(size, 0.0f);
  }
};

const float kScanAperture = 45;
const float kMinFrequency = 0;
const float kMaxFrequency = 21;
const float kNoise = 0.25f;
const float kDistanceScale = 10.0f;

inline void GatherSignal(ScannerOutput& output, Signature const& s) {
  float invBufferSize = 1.0f / (float)output.bands.size();
  for (size_t i = 0; i < output.bands.size(); ++i) {
    float t = (float)i * invBufferSize;
    float f = Mix(kMinFrequency, kMaxFrequency, t);
    output.bands[i] += 
      s.amplitude * Exp(-Pow(Abs(s.frequency - f) / s.variance, s.power));
  }
}

void GatherSignalsObject(
  ScannerOutput& output,
  Position const& pos,
  V3 const& look,
  Object const& object)
{
  Signature s = object->GetSignature();
  if (s.amplitude > 0) {
    V3 toObject = object->GetPos() - pos;
    float angle = Degrees(Angle(toObject, look));
    float dist = Length(toObject) / kDistanceScale;
    s.amplitude *=
      Exp(-16.0f * Max(0.0f, angle - 0.125f * kScanAperture)) *
      Pow(50000.0f / dist, 2.0f);

    if (s.amplitude > 0.01f)
      GatherSignal(output, s);
  }

  for (Object child = object->children; child; child = child->nextSibling)
    GatherSignalsObject(output, pos, look, child);
}

void GatherSignalsInterior(
  ScannerOutput& output,
  Position const& pos,
  V3 const& look,
  Object const& container)
{
  for (InteriorIterator it = Object_GetInteriorObjects(container);
       it.HasMore(); it.Advance())
    GatherSignalsObject(output, pos, look, it.Get());
}

FreeFunction(ScannerOutput, Object_GetScannerOutput,
  "Return the total gathered signal of 'scanner' with 'bands' frequency bands",
  Object, scanner,
  uint, bands)
{
  Object root = scanner->GetRoot().t;
  ScannerOutput output(bands);
  Position pos = root->GetPos();
  V3 look = root->GetLook();

  GatherSignalsInterior(output, pos, look, root->GetContainer().t);

  for (size_t i = 0; i < output.bands.size(); ++i)
    output.bands[i] += kNoise * RandExp();

  return output;
} FunctionAlias(Object_GetScannerOutput, GetScannerOutput);

FreeFunction(ScannerOutput, Object_GetSignature,
  "Return the raw scanner signature of 'object' with 'bands' frequency bands",
  Object, object,
  uint, bands)
{
  ScannerOutput output(bands);
  GatherSignal(output, object->GetSignature());

  for (size_t i = 0; i < output.bands.size(); ++i)
    output.bands[i] += kNoise * RandExp();

  return output;
} FunctionAlias(Object_GetSignature, GetSignature);

FreeFunction(float, ScannerOutput_AverageAmplitude,
  "Return the average amplitude of all frequency bands in 'output'",
  ScannerOutput, output)
{
  float total = 0;
  for (size_t i = 0; i < output.bands.size(); ++i)
    total += output.bands[i];

  return total / (float)output.bands.size();
} FunctionAlias(ScannerOutput_AverageAmplitude, AverageAmplitude);

FreeFunction(float, ScannerOutput_AverageFrequency,
  "Return the amplitude-weighted average frequency of 'output'",
  ScannerOutput, output)
{
  float total = 0;
  float totalAmplitude = 0;
  for (size_t i = 0; i < output.bands.size(); ++i) {
    float f = (float)i / (float)output.bands.size();
    totalAmplitude += output.bands[i];
    total += f * output.bands[i];
  }

  return total / Max(0.0001f, totalAmplitude);
} FunctionAlias(ScannerOutput_AverageFrequency, AverageFrequency);

VoidFreeFunction(ScannerOutput_Blend,
  "Mix 'source's output into 'target' with 'interpolant' blend factor",
  ScannerOutput, target,
  ScannerOutput, source,
  float, interpolant)
{
  if (target.bands.size() != source.bands.size())
    error("Cannot mix scanner outputs of different resolutions")
  for (size_t i = 0; i < target.bands.size(); ++i)
    Mutable(target).bands[i] = Mix(
      target.bands[i], source.bands[i], interpolant);
} FunctionAlias(ScannerOutput_Blend, Blend);

FreeFunction(float, ScannerOutput_Get,
  "Return the raw 'i'th frequency band amplitude in 'output'",
  ScannerOutput, output,
  int, i)
{
  return output.bands[i];
} FunctionAlias(ScannerOutput_Get, Get);

FreeFunction(int, ScannerOutput_Size,
  "Return the number of frequency bands in 'output'",
  ScannerOutput, output)
{
  return (int)output.bands.size();
} FunctionAlias(ScannerOutput_Size, Size);
