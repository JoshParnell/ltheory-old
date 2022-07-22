#include "BuildMode.h"
#ifdef BUILD_DEBUG
#define LTE_CONSOLE
#endif

#include "LTE/LTE.h"

#include "Component/Resources.h"

#include "Game/Camera.h"
#include "Game/Items.h"
#include "Game/Objects.h"
#include "Game/Player.h"
#include "Game/RenderPasses.h"
#include "Game/Universe.h"
#include "Game/Widgets.h"

#include "Module/FrameTimer.h"
#include "Module/MusicEngine.h"
#include "Module/PhysicsEngine.h"
#include "Module/Settings.h"
#include "Module/SoundEngine.h"

#include "UI/Compositors.h"
#include "UI/Interface.h"
#include "UI/Widgets.h"

// #define RANDOM_UNIVERSE
const uint kUniverseSeed = 98080;
const Quantity kShipValue = 10000;
const float kSimulationFrequency = 30;

void Screenshot() {
  uint index = 0;
  String prefix = OS_GetUserDataPath() + "screenshot/";
  OS_CreatePath(prefix);
  String path;
  while (true) {
    path = prefix + ToString(index) + ".png";
    if (!OS_FileExists(path))
      break;
    index++;
  }

  Texture_ScreenCapture()->SaveTo(path);
}

struct LTheory : public Program {
  Object universe;
  Player player;
  Camera camera;
  Interface worldView;
  Interface interface;
  float interpolant;

  LTheory() : interpolant(0) {
    window = Window_Create(
      "Limit Theory",
      V2U(Config_Int("screenwidth", "Graphics", 1280),
          Config_Int("screenheight", "Graphics", 768)),
      true,
      Config_Bool("fullscreen", "Graphics", false));

    window->SetCaptureMouse(true);
    window->SetSync(Config_Bool("enablevsync", "Graphics", true));
    Renderer_Initialize();
  }

  void Reload() {
    if (!interface)
      interface = Interface_Create("UI");
    interface->Clear();
    Script_ClearCache();

    /* Create the primary interface. */ {
      Widget widget;
      ScriptFunction_Load("Widget/HUD:Create")->Call(widget, player);
      LTE_ASSERT(widget);
      interface->Add(widget);
    }
  }

  void OnInitialize() {
    Renderer_Clear();
    window->Display();
    Mouse_SetPos(window->GetSize() / 2);

    Profiler_AutoProfile(5);

    AddModule(CreatePhysicsEngine());
    AddModule(Config_Bool("enableaudio", "General", true)
      ? SoundEngine_Fmod()
      : SoundEngine_Null());
    // AddModule(CreateMusicEngine());

    Setup();
    Reload();

    /* Create the primary render view. */
    camera = Camera_Create();
    Camera_Push(camera);

#if 0
    Vector<RenderPass> passes;
    passes.push(RenderPass_Clear(0));
    passes.push(RenderPass_Camera(camera));
    passes.push(RenderPass_SMAA());
    passes.push(RenderPass_Interface(interface));
    passes.push(RenderPass_Bloom(64, 32));
    passes.push(RenderPass_PostFilter("post/dither.jsl"));
    AddModule(Module_Viewer(window, RenderPass_Composite(passes)));
#else
    Vector<RenderPass> passes;
    passes.push(RenderPass_Clear(0));
    passes.push(RenderPass_Camera(camera));
    passes.push(RenderPass_SMAA());
    passes.push(RenderPass_Interface(interface));
    passes.push(RenderPass_Bloom(64, 32));
    passes.push(RenderPass_PostFilter("post/dither.jsl"));

    worldView = Interface_Create("Game View");
    worldView->Add(Widget_Rendered(passes));
#endif

    Sound_Play2D("system/ambiance/002.wav", 0.02f, true);
    Sound_Play2D("system/ambiance/037.wav", 0.01f, true);
  }

  void Setup() {
#ifdef RANDOM_UNIVERSE
    uint seed = rand() % 100000;
#else
    uint seed = kUniverseSeed;
#endif
    universe = Object_Universe(seed, 1);
    printf("Creating universe %d\n", seed);

    Object base = universe->GetInteriorObjects();
    while (base->GetType() != ObjectType_System)
      base = base->GetInteriorObjects();

    /* Ship. */
    Object ship = Item_ShipType(kShipValue, 51)->Instantiate();
    Object home = base->GetInteriorObjects(ObjectType_Station);
    if (home)
      ship->SetPos(home->GetPos() + Position(20000, 0, 0));
    ship->SetPos(ship->GetPos() + Position(0, 250000, 0));
    base->AddInterior(ship);
    ship->SetLook(Normalize(V3(-0.5f, 0, 0.5f)));

    /* Transfer unit. */
#if 0
      Item tu = Item_TransferUnitType(1, 1, 1, 1, 1, 1);
      if (!ship->Plug(tu))
        dbg | "Failed to plug transfer unit." | endl;
#endif

      /* Weapons. */
#if 1
      Item weapon = Item_WeaponType(40);
      while (ship->Plug(weapon)) {}
      ship->AddItem(weapon, 1);
#endif

      /* Random cargo. */
#if 1
      for (uint i = 0; i < 4; ++i)
        ship->AddItem(base->GetResources()->elements.sample(Rand()), 1);
#endif

      /* Drone Bay. */
#if 0
      Item bay = Item_DroneBayType(1, 1);
      if (!ship->Plug(bay))
        dbg | "Failed to plug drone bay." | endl;
#endif

    /* Player. */ {
      player = Player_Human();
      player->AddCredits(1000);
      player->AddAsset(ship);
      player->Pilot(ship);
      player->SetName("Trent Edison");
    }

    /* Other ship. */ {
      Object ship2 = Item_ShipType(200000000, 49)->Instantiate();
      // ship2->AddItem(Item_Blueprint(Meta_TransferUnitType(1, 5, 1, 1, 1, 1)), 1);
      ship2->Plug(Item_ProductionLabType(1, 1));
      ship2->Plug(Item_TechLabType(1, 1));
      ship2->SetName("Voyager");
      for (uint i = 0; i < 1200; ++i)
        ship2->AddItem(base->GetResources()->elements.sample(Rand()), 1);
      ship2->SetPos(ship->GetPos() + RandDirection() * 1000.0f);
      base->AddInterior(ship2);
      player->AddAsset(ship2);
    }
  }

  void OnUpdate() {
    float dt = FrameTimer_Get();
    camera->SetTarget(player->piloting);

#ifdef BUILD_DEBUG
    if (Keyboard_Pressed(Key_M))
      player->AddCredits(1000000);
    if (Keyboard_Pressed(Key_K))
      player->piloting->AddItem(Item_WeaponType(rand()), rand() % 3);
    if (Keyboard_Down(Key_H))
      dt *= 10;
    if (Keyboard_Control() && Keyboard_Pressed(Key_W))
      deleted = true;
    if (Keyboard_Pressed(Key_F1))
      Screenshot();
    if (Keyboard_Pressed(Key_F2))
      Profiler_AutoProfile(1.0f);
    if (Keyboard_Pressed(Key_F5))
      Reload();
#endif

    if (player) {
      Object container = player->piloting->GetContainer().t;
      UpdateState state(dt, true);
      state.dt = 1.0f / kSimulationFrequency;

      interpolant += kSimulationFrequency * dt;
      while (interpolant >= 1.0f) {
        interpolant -= 1.0f;
        container->Update(state);
      }

      container->SetInterpolant(interpolant);
      Universe_Get()->age += state.quanta;
    }

    interface->Update();
    worldView->Update();
    worldView->Draw();
  }
};

int main(int argc, char const* argv[]) {
  LTheory().Execute();
  return 0;
}
