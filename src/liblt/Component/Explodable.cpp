#include "Explodable.h"
#include "Integrity.h"

#include "Game/Graphics/Effects.h" 
#include "Game/Object.h"

void ComponentExplodable::Run(ObjectT* self, UpdateState& state) {
  ComponentIntegrity* it = self->GetIntegrity();
  LTE_ASSERT(it);

  if (!exploded && it->health <= 0) {
    exploded = true;
    if (explosionType != ExplosionType_SIZE)
      Effect_MultiExplosionRadial(self, 1, explosionType);
  }
}
