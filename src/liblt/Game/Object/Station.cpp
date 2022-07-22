#include "../Objects.h"

#include "Component/Affectable.h"
#include "Component/Asset.h"
#include "Component/BoundingBox.h"
#include "Component/Cargo.h"
#include "Component/Collidable.h"
#include "Component/Crew.h"
#include "Component/Cullable.h"
#include "Component/Detectable.h"
#include "Component/Dockable.h"
#include "Component/Drawable.h"
#include "Component/Explodable.h"
#include "Component/Integrity.h"
#include "Component/Interior.h"
#include "Component/Market.h"
#include "Component/MissionBoard.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Pilotable.h"
#include "Component/Sockets.h"
#include "Component/Storage.h"
#include "Component/Supertyped.h"
#include "Component/Targets.h"
#include "Component/Zoned.h"

#include "Game/Item/StationType.h"

#include "LTE/DrawState.h"
#include "LTE/Grammar.h"
#include "LTE/Model.h"
#include "LTE/Pointer.h"
#include "LTE/Pool.h"
#include "LTE/RNG.h"
#include "LTE/RenderStyle.h"
#include "LTE/SDFs.h"

typedef ObjectWrapper
  < Component_Affectable
  < Component_Asset
  < Component_BoundingBox
  < Component_Cargo
  < Component_Collidable
  < Component_Crew
  < Component_Cullable
  < Component_Detectable
  < Component_Dockable
  < Component_Drawable
  < Component_Explodable
  < Component_Integrity
  < Component_Interior
  < Component_Market
  < Component_MissionBoard
  < Component_Nameable
  < Component_Orientation
  < Component_Pilotable
  < Component_Sockets
  < Component_Storage
  < Component_Supertyped
  < Component_Targets
  < Component_Zoned
  < ObjectWrapperTail<ObjectType_Station>
  > > > > > > > > > > > > > > > > > > > > > > > >
  StationBaseT;

AutoClassDerivedEmpty(Station, StationBaseT)
  DERIVED_TYPE_EX(Station)
  POOLED_TYPE

  void Initialize() {
    Interior.allowMovement = false;
    Zoned.region = SDF_Sphere(0, Length(0.5f * GetLocalBound().GetSideLengths()));
  }

  void BeginDrawInterior(DrawState* state) {
    GetContainer()->BeginDrawInterior(state);
  }

  void OnDrawInterior(DrawState* state) {
    RenderStyle_Get()->SetTransform(Transform_Scale(2000));
    ((StationType*)(ItemT*)Supertyped.type)->interiorModel->Render(state);
  }

  void EndDrawInterior(DrawState* state) {
    GetContainer()->EndDrawInterior(state);
  }
};

DERIVED_IMPLEMENT(Station)

DefineFunction(Object_Station) {
  Reference<Station> self = new Station;
  self->SetSupertype(args.type);
  self->Initialize();
  return self;
}
