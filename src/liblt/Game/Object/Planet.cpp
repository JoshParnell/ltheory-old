#include "Planet.h"

#include "Component/Resources.h"

#include "Game/Items.h"
#include "Game/Objects.h"

#include "LTE/DrawState.h"
#include "LTE/Grammar.h"
#include "LTE/Location.h"
#include "LTE/Math.h"
#include "LTE/Model.h"
#include "LTE/Meshes.h"
#include "LTE/RNG.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

#include "LTE/Debug.h"

AutoClassDerived(PlanetImpl, Planet,
  double, hunger,
  double, population)

  DERIVED_TYPE_EX(PlanetImpl)

  PlanetImpl() :
    hunger(0),
    population(1)
    {}

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

#if 0
    if (!orbitalStation) {
      orbitalStation = Item_StationType(500000000, 310)->Instantiate();
      float angle = 0;
      orbitalStation->SetPos(
        GetPos() + 2.5f * GetScale().x * V3(Cos(angle), 0, Sin(angle)));
      orbitalStation->SetName(GetName() + " Orbital Station");
      manager->AddAsset(orbitalStation);
      GetContainer()->AddInterior(orbitalStation);
    }
#endif
    
    /* Popoulation dynamics. */
#if 0
    const double kGrowthRate = 0.03;
    const double kMinGrowth = 0.5;
    const double kMaxGrowth = 2.0;
    const double kHunger = 0.1;
    const double kMaxPopulation = 10000;

    hunger += kHunger * population * state.dt;
    double h = hunger / population;
    double growth = Mix(kMinGrowth, kMaxGrowth, Exp(-h));
    growth = Mix(growth, 1.0, Exp(-kGrowthRate * state.dt));
    population *= growth;
    population = Clamp(population, 1.0, kMaxPopulation);

    if (growth < 1.0)
      hunger *= growth;

#if 0
    dbg
      | " Hunger = " | hunger
      | " Population = " | population
      | " H / P = " | h
      | " Growth = " | growth | endl;
#endif

    Item item = GetContainer()->GetResources()->elements.entries[0].key;
    Pointer<ComponentOrders> orders = manager->GetOrders();
    Quantity current = 0;
    for (size_t i = 0; i < orders->elements.size(); ++i)
      current += orders->elements[i]->volume;

    Quantity volume = (Quantity)hunger;
    if (current < volume) {
      Quantity price = (Quantity)(100.0 * h);
      Order order = Order_Create(manager, item, volume - current, price);
      orbitalStation->AddMarketBid(order);
    }
    
    Quantity food = orbitalStation->GetStorageLocker(manager)->GetItemCount(item);
    if (hunger > 0 && food > 0) {
      orbitalStation->GetStorageLocker(manager)->RemoveItem(item, food);
      hunger = Max(0.0, hunger - (double)food);
    }
#endif
  }
};

DERIVED_IMPLEMENT(PlanetImpl)

DefineFunction(Object_Planet) {
  Reference<PlanetImpl> self = new PlanetImpl;
  self->Seeded.seed = args.type->GetSeed();

  RNG rg = RNG_MTG(self->GetSeed());
  self->SetName(String_Capital(Grammar_Get()->Generate(rg, "$system", "")));
  self->SetSupertype(args.type);

  self->manager = Player_AI(Traits());
  self->manager->AddCredits(1000000000);
  self->manager->SetName(self->GetName() + " Governor");
  return self;
}
