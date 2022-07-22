#include "Task.h"
#include "Object.h"

#include <limits>

Quantity TaskT::CanPerform(Object const& owner) const {
  Quantity cycles = std::numeric_limits<Quantity>::max();

  static Vector<ItemDelta> inputs;
  inputs.clear();
  GetInputs(inputs);
  for (size_t i = 0; i < inputs.size(); ++i) {
    ItemDelta const& input = inputs[i];
    cycles = Min(cycles,
      input.node->GetStorageLocker(owner)->GetItemCount(input.item) / input.delta);
  }

  return cycles;
}

Object TaskT::GetTarget() const {
  return nullptr;
}
