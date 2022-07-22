#include "Serializer.h"
#include "Array.h"
#include "Location.h"
#include "Map.h"
#include "VectorMap.h"

#include "Debug.h"

namespace {
  typedef Map<void*, int> IdMapT;
  typedef Map<int, void*> ResourceMapT;

  struct ResourceMap {
    ResourceMapT resources;
    IdMapT ids;

    int AllocateID(void* resource) {
      int thisID = (int)resources.size();
      resources[thisID] = resource;
      ids[resource] = thisID;
      return thisID;
    }

    int const* GetID(void* resource) const {
      return ids.get(resource);
    }

    void* Get(int id) const {
      void* const* res = resources.get(id);
      return res ? *res : nullptr;
    }

    void Set(int id, void* resource) {
      resources[id] = resource;
    }
  };

  struct LoaderImpl :
    public Serializer,
    public FieldMapper
  {
    Location const& location;
    int version;
    AutoPtr< Array<uchar> > buffer;
    ResourceMap resources;
    size_t cursor;
    bool good;

    LoaderImpl(Location const& location) :
      location(location),
      cursor(0),
      good(true)
    {
      buffer = location->Read();
      if (buffer)
        Read(version);
      else
        good = false;
    }

    bool IsGood() const {
      return good;
    }

    int GetVersion() const {
      return version;
    }

    void Read(char* data, size_t size) {
      if ((cursor + size) <= buffer->size()) {
        memcpy(data, &(*buffer)[cursor], size);
        cursor += size;
      } else
        good = false;
    }

    template <class T>
    void Read(T& t) {
      Read((char*)&t, sizeof(t));
    }

    void operator()(void* data, char const* name, Type const& type, void*) {
      Process(data, type);
    }

    void ProcessPointer(void* data, Type const& type) {
      int resourceID;
      Read(resourceID);
      if (resourceID < 0) {
        *((void**)data) = nullptr;
        return;
      }

      *((void**)data) = resources.Get(resourceID);

      if (*((void**)data)) {
        return;
      } else {
        FieldType pointee = type->GetField(data, 0);

        /* If the type is abstract, drill down until we get a concrete type. */
        while (pointee.type->IsAbstract()) {
          String name;
          Process(name);

          bool located = false;
          Vector<Type> const& derived = pointee.type->GetDerived();
          for (size_t i = 0; i < derived.size(); ++i) {
            if (derived[i]->name == name) {
              pointee.type = derived[i];
              located = true;
              break;
            }
          }

          if (!located)
            error("Failed to locate ID of dynamic derived type");
        }

        *((void**)data) = pointee.type->Allocate();
        resources.Set(resourceID, *((void**)data));
        type->Map(data, *this, nullptr);
      }
    }

    template <class T>
    void Process(T const& t) {
      Process((void*)&t, Type_Get(t));
    }

    void Process(void* data, Type const& type) {
      if (!type)
        return;

      if (type == Type_Get<Array<uchar> >() ||
          type == Type_Get<Array<char> >())
      {
        Array<char>* arr = (Array<char>*)data;
        Read(arr->_size);
        arr->resize(arr->_size);
        Read(arr->data(), arr->size());
      } else if (type->IsPrimitive())
        Read((char*)data, type->size);
      else if (type->IsPointer())
        ProcessPointer(data, type);
      else
        type->Map(data, *this, nullptr);
    }
  };

  struct SaverImpl :
    public Serializer,
    public FieldMapper
  {
    Location const& location;
    int version;

    AutoPtr< Array<uchar> > buffer;
    ResourceMap resources;
    size_t cursor;
    bool good;

    SaverImpl(Location const& location, int version = 0) :
      location(location),
      version(version),
      cursor(0),
      good(true)
    {
      buffer = new Array<uchar>(16);
      Write(this->version);
    }

    ~SaverImpl() {
      Array<uchar> writeBuffer(cursor);
      memcpy(writeBuffer.data(), buffer->data(), cursor);
      location->Write(writeBuffer);
    }

    bool IsGood() const {
      return good;
    }

    int GetVersion() const {
      return version;
    }

    void Write(char const* data, size_t size) {
      // TODO : You're so lazy.
      while ((cursor + size) >= buffer->size()) {
        Array<uchar> temp(cursor);
        memcpy(temp.data(), buffer->data(), cursor);
        buffer->resize(buffer->size() * 2);
        memcpy(buffer->data(), temp.data(), cursor);
      }

      memcpy(&(*buffer)[cursor], data, size);
      cursor += size;
    }

    template <class T>
    void Write(T const& t) {
      Write((char const*)&t, sizeof(t));
    }

    void operator()(void* data, char const* name, Type const& type, void*) {
      Process(data, type);
    }

    void ProcessPointer(void* data, Type const& type) {
      void* ptr = *((void**)data);

      /* nullptr -> -1 */
      if (!ptr) {
        int id = -1;
        Write(id);
        return;
      }

      /* Check for existing resources. */
      int const* existingID = resources.GetID(ptr);
      if (existingID) {
        Write(*existingID);
        return;
      }

      /* New resource, must traverse. */
      int id = resources.AllocateID(ptr);
      Write(id);

      /* Write derived type name if abstract. */
      FieldType pointee = type->GetField(data, 0);
      if (pointee.type->IsAbstract()) {
        FieldType derived = pointee.type->GetField(pointee.address, 0);
        Process(derived.type->name);
      }

      type->Map(data, *this, nullptr);
    }

    template <class T>
    void Process(T const& t) {
      Process((void*)&t, Type_Get(t));
    }

    void Process(void* data, Type const& type) {
      if (!type)
        return;

      if (type == Type_Get<Array<uchar> >() ||
          type == Type_Get<Array<char> >())
      {
        Array<char>* arr = (Array<char>*)data;
        Write(arr->_size);
        Write(arr->data(), arr->_size);
      } else if (type->IsPrimitive())
        Write((char*)data, type->size);
      else if (type->IsPointer())
        ProcessPointer(data, type);
      else
        type->Map(data, *this, nullptr);
    }
  };
}

namespace LTE {
  Serializer* CreateLoader(Location const& location) {
    return new LoaderImpl(location);
  }

  Serializer* CreateSaver(Location const& location, int version) {
    return new SaverImpl(location, version);
  }
}
