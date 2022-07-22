#include "BuildMode.h"
#ifdef BUILD_DEBUG
  #define LTE_CONSOLE
#endif

#if 0
#include "LTE/LTE.h"
#include "Module/SoundEngine.h"

const uint kMinWidth = 800;
const uint kMinHeight = 640;
const uint kMinBitsPerPixel = 32;

const int kCurrentVersion = 110;

struct UpdateChecker : public JobT {
  enum State {
    Checking,
    Failure,
    Latest,
    Available
  } state;

  UpdateChecker() :
    state(Checking)
    {}

  void operator()(uint units) {
    Location versionFile = Location_Web("http://ltheory.com", "update/currentversion.txt");
    String versionString = versionFile->ReadAscii();
    if (!versionString.size()) {
      state = Failure;
      return;
    }

    int version = FromString<int>(versionString);
    if (version == kCurrentVersion) {
      state = Latest;
      return;
    }
  }

  virtual char const* GetName() const {
    return "Update Check";
  }
};

struct Launcher : public Program {
  int videoMode;
  int meshQuality;
  bool fullscreen;
  bool audio;
  bool vsync;

  Launcher() :
    videoMode(0),
    meshQuality(Config_Int("meshquality", "Graphics", 2)),
    fullscreen(Config_Bool("fullscreen", "Graphics", true)),
    audio(Config_Bool("enableaudio", "General", true)),
    vsync(Config_Bool("enablevsync", "General", true))
  {
    window.width = 840;
    window.height = 480;
    window.title = "Limit Theory Launcher";
    hasCaption = true;
    Renderer_Initialize();
  }

  void Submit() {
    sf::VideoMode selectedMode = sf::VideoMode::getFullscreenModes()[videoMode];
    Config_Set("fullscreen", "Graphics", fullscreen ? "true" : "false");
    Config_Set("enableaudio", "General", audio ? "true" : "false");
    Config_Set("enablevsync", "Graphics", vsync ? "true" : "false");
    Config_Set("screenwidth", "Graphics", ToString(selectedMode.width));
    Config_Set("screenheight", "Graphics", ToString(selectedMode.height));
    Config_Set("screenbpp", "Graphics", ToString(selectedMode.bitsPerPixel));
    Config_Set("meshquality", "Graphics", ToString(meshQuality));
    Config_Flush();
  }

  void Initialize() {
    SetIcon("icon.png");
    window.setVerticalSyncEnabled(true);
    Mouse_SetVisible(false);

    modules.push(&UIManager::Get());
    modules.push(SoundEngine_Null());

    const std::vector<sf::VideoMode>& modes = sf::VideoMode::getFullscreenModes();
    std::vector<Widget*> widgets;
    Widget* ddGroup = CreateUIGroupY()
      ->SetPadding(10);
    
    uint currentWidth = Config_Int("screenwidth", "Graphics", 0);
    uint currentHeight = Config_Int("screenheight", "Graphics", 0);
    uint currentBPP = Config_Int("screenbpp", "Graphics", 0);

    for (size_t i = 0; i < modes.size(); ++i) {
      if (modes[i].width < kMinWidth || modes[i].height < kMinHeight)
        continue;
      Widget* thisGroup = CreateUIPanel()
        ->Add(CreateUILabel(Stringize() | modes[i].width | " x "
                                        | modes[i].height | " ("
                                        | modes[i].bitsPerPixel | ")"));
      widgets.push_back(thisGroup);
      ddGroup->Add(thisGroup);
      if (currentWidth == modes[i].width &&
          currentHeight == modes[i].height &&
          currentBPP == modes[i].bitsPerPixel)
        videoMode = i;
    }

    CreateUIRadioGroup(widgets, &videoMode);
    Widget* videoModeDropdown = CreateUIDropDown("Video Mode")->Add(ddGroup);

    widgets.clear();
    const char* labels[5] = {
      "LOW", "NORMAL", "HIGH", "VERY HIGH", "ULTRA",
    };
    CreateUIRadioGroup(widgets, &meshQuality);
    Widget* meshQualityDropdown = CreateUIDropDown("Model Detail")->Add(ddGroup);
  }

  void Exit(UIClickData const& t) {
    Submit();
    exit = true;
  }

  void Launch(UIClickData const& t) {
    Submit();
    if (!OS_Spawn("./ltheory"))
      CreateUIMessageBox("ERROR", "Failed to launch Limit Theory. Please try launching manually.");
    else
      exit = true;
  }

  void Update() {
    Renderer_Clear();
  }
};
#endif

int main(int argc, char const* argv[]) {
  //Launcher().Start();
  return 0;
}
