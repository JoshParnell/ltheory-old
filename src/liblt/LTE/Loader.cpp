#include "Loader.h"
#include "Module.h"
#include "Pointer.h"
#include "Vector.h"

namespace {
  struct LoaderHandler : public ModuleT {
    Vector<void (*)()> toLoad;
    Vector<void (*)()> toUnload;

    ~LoaderHandler() {
      for (size_t i = 0; i < toUnload.size(); ++i)
        toUnload[i]();
    }

    char const* GetName() const {
      return "Loader Handler";
    }

    void Update() {
      for (size_t i = 0; i < toLoad.size(); ++i)
        toLoad[i]();
      toLoad.clear();
    }
  };

  Reference<LoaderHandler>& GetHandler() {
    static Reference<LoaderHandler> handler;
    if (!handler) {
      handler = new LoaderHandler;
      Module_RegisterGlobal(handler);
    }
    return handler;
  }
}

namespace LTE {
  bool RegisterLoader(void (*loader)()) {
    GetHandler()->toLoad.push(loader);
    return true;
  }

  bool RegisterUnloader(void (*unloader)()) {
    GetHandler()->toUnload.push(unloader);
    return true;
  }
}
