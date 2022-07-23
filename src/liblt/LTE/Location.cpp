#include "Location.h"
#include "Archive.h"
#include "Array.h"
#include "AutoClass.h"
#include "AutoPtr.h"
#include "OS.h"
#include "Pool.h"
#include "ProgramLog.h"
#include "ResourceMap.h"
#include "Vector.h"

#include "BuildMode.h"

#include "SFML/Network.hpp"

#include <fstream>

#include "LTE/Debug.h"

#ifdef BUILD_RELEASE
  const bool kUseArchive = true;
  const String kResourcePath = "./";
#else
  const bool kUseArchive = false;
  const String kResourcePath = "resource/";
#endif

const String kArchivePath = kResourcePath + "resources.bin";
const String kModPath = "./mod/";

TypeAlias(Reference<LocationT>, Location);

namespace {
  AutoPtr<Archive> gArchive;

  void InitializeArchive() {
    if (!gArchive) {
      gArchive = Archive::Open(kArchivePath);
      if (!gArchive->GetFileCount())
        Log_Critical("Failed to load resource archive " + kArchivePath);
    }
  }

  ResourceMap const& GetResourceMap() {
    static ResourceMap resourceMap;
    if (!resourceMap) {
      resourceMap = ResourceMap_Create();
      resourceMap->AddDirectory(kResourcePath, "");

      Vector<String> mods = OS_ListDir(kModPath);
      for (size_t i = 0; i < mods.size(); ++i) {
        String const& mod = mods[i];
        if (mod == "." || mod == "..")
          continue;

        String fullPath = kModPath + mod;
        if (OS_IsDir(fullPath))
          resourceMap->AddDirectory(fullPath + "/");
      }
    }
    return resourceMap;
  }

  AutoClassDerived(LocationFile, LocationT,
    String, path)
    DERIVED_TYPE_EX(LocationFile)
    POOLED_TYPE

    LocationFile() {}

    Location Clone() const {
      return new LocationFile(*this);
    }

    bool Exists() const {
      std::ifstream stream(path.c_str(), std::ios::binary);
      return stream.good();
    }

    AutoPtr< Array<uchar> > Read() const {
      std::ifstream stream(path.c_str(), std::ios::binary);
      if (!stream || !OS_IsFile(path)) {
        Log_Warning("Failed to open file <" + path + "> for reading");
        return nullptr;
      }

      stream.seekg(0, std::ios_base::end);
      size_t size = (size_t)stream.tellg();
      stream.seekg(0);

      char* buf = new char[size];
      if (!stream.read(buf, size)) {
        free(buf);
        Log_Warning("Failed to read file <" + path + ">");
        return nullptr;
      }
      return (new Array<uchar>)->set((uchar*)buf, size);
    }

    String ToString() const {
      return path;
    }

    bool Write(Array<uchar> const& data) const {
      std::ofstream stream(path.c_str(), std::ios::binary);
      if (!stream) {
        OS_CreatePath(path.c_str());
        stream.close();
        stream.clear();
        stream.open(path.c_str(), std::ios::binary);
        if (!stream) {
          Log_Warning("Failed to open file <" + path + "> for writing");
          return false;
        }
      }
      stream.write((const char*)data.data(), data.size());
      return stream.good();
    }
  };

  DERIVED_IMPLEMENT(LocationFile)

  AutoClassDerived(LocationMemory, LocationT,
    Array<uchar>*, memory,
    bool, ownsMemory)
    DERIVED_TYPE_EX(LocationMemory)
    POOLED_TYPE

    LocationMemory() :
      memory(new Array<uchar>),
      ownsMemory(true)
      {}

    LocationMemory(Array<uchar>* memory) :
      memory(memory),
      ownsMemory(false)
      {}

    LocationMemory(String const& str) :
      memory(new Array<uchar>),
      ownsMemory(true)
    {
      memory->resize(str.size());
      memcpy(memory->data(), &str.front(), str.size());
    }

    ~LocationMemory() {
      if (ownsMemory)
        delete memory;
    }

    Location Clone() const {
      return new LocationMemory(*this);
    }

    bool Exists() const {
      return true;
    }

    AutoPtr< Array<uchar> > Read() const {
      Array<uchar>* arr = new Array<uchar>;
      *arr = *memory;
      return arr;
    }

    String ToString() const {
      return "[Memory]";
    }

    bool Write(const Array<uchar>& data) const {
      *((LocationMemory*)this)->memory = data;
      return true;
    }
  };

  DERIVED_IMPLEMENT(LocationMemory)

  AutoClassDerived(LocationResource, LocationT,
    String, name)
    DERIVED_TYPE_EX(LocationResource)
    POOLED_TYPE

    LocationResource() {}

    Location Clone() const {
      return new LocationResource(*this);
    }

    bool Exists() const {
      if (kUseArchive) {
        InitializeArchive();
        if (gArchive->Contains(name))
          return true;
      }

      return GetResourceMap()->Exists(name);
    }

    AutoPtr< Array<uchar> > Read() const {
      if (kUseArchive) {
        InitializeArchive();
        if (gArchive->Contains(name))
          return gArchive->Read(name);
      }

      return LocationFile(GetResourceMap()->Get(name)).Read();
    }

    String ToString() const {
      return kResourcePath + name;
    }

    bool Write(Array<uchar> const& data) const {
      if (kUseArchive) {
        InitializeArchive();
        if (gArchive->Contains(name))
          return gArchive->Write(name, data);
      }

      return LocationFile(GetResourceMap()->Get(name)).Write(data);
    }
  };

  DERIVED_IMPLEMENT(LocationResource)

  AutoClassDerived(LocationWeb, LocationT,
    String, host,
    String, item)
    DERIVED_TYPE_EX(LocationWeb)
    POOLED_TYPE

    LocationWeb() {}

    Location Clone() const {
      return new LocationWeb(*this);
    }

    bool Exists() const {
      return true;
    }

    bool Write(Array<uchar> const& data) const {
      LTE_ASSERT(!data.size());
      return false;
    }

    AutoPtr< Array<uchar> > Read() const {
      sf::Http http(host);
      sf::Http::Response response = http.sendRequest(sf::Http::Request(item));
      if (response.getStatus() != sf::Http::Response::Ok)
        return nullptr;

      String message = response.getBody();
      Array<uchar>* arr = new Array<uchar>(message.size());
      memcpy(arr->data(), &message.front(), message.size());
      return arr;
    }

    String ToString() const {
      return host + "/" + item;
    }
  };

  DERIVED_IMPLEMENT(LocationWeb)
}

namespace LTE {
  HashT LocationT::GetHash() const {
    return String_Hash(ReadAscii());
  }

  String LocationT::ReadAscii() const {
    AutoPtr< Array<uchar> > arr = Read();
    String str;
    if (!arr)
      return str;

    str.reserve(arr->size());
    for (size_t i = 0; i < arr->size(); ++i) {
      char c = (*arr)[i];
      if (c != '\r')
        str.push_back(c);
    }
    return str;
  }

  DefineFunction(Location_Cache) {
    return Location_File(OS_GetUserDataPath() + "cache/" + args.name);
  }

  DefineFunction(Location_File) {
    return new LocationFile(args.file);
  }

  Location Location_Memory(String const& data) {
    return new LocationMemory(data);
  }

  Location Location_Memory(Array<uchar>* memory, bool ownsMemory) {
    return new LocationMemory(memory, ownsMemory);
  }

  DefineFunction(Location_Resource) {
    return new LocationResource(args.name);
  }

  DefineFunction(Location_Web) {
    return new LocationWeb(args.host, args.file);
  }
}
