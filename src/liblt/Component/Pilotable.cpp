#include "Pilotable.h"
#include "Integrity.h"

void ComponentPilotable::Run(ObjectT* self, UpdateState& state) {
  if (pilot) {
    pilot->Update(state);

    if (!self->IsAlive()) {
      /* TODO : What if player dies? */
      pilot = nullptr;
    }
  }
}
