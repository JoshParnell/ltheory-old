#include "Settings.h"
#include "SettingsEntry.h"

#include "LTE/Axis.h"
#include "LTE/AutoClass.h"
#include "LTE/Button.h"
#include "LTE/Color.h"
#include "LTE/Hash.h"
#include "LTE/Iterator.h"
#include "LTE/Keyboard.h"
#include "LTE/Location.h"
#include "LTE/Map.h"
#include "LTE/Pool.h"
#include "LTE/Pointer.h"
#include "LTE/OS.h"
#include "LTE/V3.h"
#include "LTE/Vector.h"

const int kSettingsVersion = 8;

namespace {
  AutoClass(SettingsNode,
    String, name,
    Pointer<SettingsEntry>, value,
    Pointer<SettingsNode>, head,
    Pointer<SettingsNode>, next)

    SettingsNode() {}

    SettingsNode(String const& name) :
      name(name)
      {}
  };

  typedef Map<String, SettingsNode*> NodeMapT;

  AutoClass(Settings,
    Pointer<SettingsNode>, root,
    NodeMapT, nodeMap)
    String file;

    Settings() {
      LoadFrom(*this, Location_File(GetPath()), kSettingsVersion);
      if (!root)
        root = new SettingsNode("Settings");
    }

    ~Settings() {
      SaveTo(*this, Location_File(GetPath()), kSettingsVersion);
    }

    String GetPath() const {
      return OS_GetUserDataPath() + "settings.bin";
    }

    SettingsNode& GetNode(String const& address) {
      if (!address.size())
        return *root;

      SettingsNode*& node = nodeMap[address];
      if (node)
        return *node;

      Vector<String> split;
      String_Split(split, address, '/');
      node = new SettingsNode(split.back());

      String parentAddress;
      for (size_t i = 0; i + 1 < split.size(); ++i) {
        if (i)
          parentAddress += '/';
        parentAddress += split[i];
      }

      SettingsNode& parent = GetNode(parentAddress);
      LIST_INSERT(parent.head, next, node)
      return *node;
    }
  };

  Settings& GetSettings() {
    static Settings s;
    return s;
  }

  AutoClassDerived(WidgetSettings, WidgetComponentT,
    SettingsNode*, node)

    DERIVED_TYPE_EX(WidgetSettings)
    POOLED_TYPE

    WidgetSettings() {}

    void CreateChildren(Widget const&, Vector<Widget>& widgets) {
      for (SettingsNode* i = node->head; i; i = i->next) {
        if (i->head)
          widgets.push(Widget_Create1(new WidgetSettings(i)));
        else if (i->value) {
          Widget w = i->value->GetWidget();
          if (w)
            widgets.push(w);
        }
      }
    }

    void GetHash(Widget const&, HashT& out) {
      out = Hash(node);
    }

    void GetName(Widget const&, String& out) {
      out = node->name;
    }
  };
}

GenericAxis Settings_Axis(String const& name, Axis const& defValue) {
  SettingsNode& node = GetSettings().GetNode(name);
  if (!node.value)
    node.value = SettingsEntry_Axis(node.name, defValue);

  GenericAxis g;
  node.value->GetValue((void*)&g);
  return g;
}

GenericBool Settings_Bool(String const& name, bool defValue) {
  SettingsNode& node = GetSettings().GetNode(name);
  if (!node.value)
    node.value = SettingsEntry_Bool(node.name, defValue);

  GenericBool g;
  node.value->GetValue((void*)&g);
  return g;
}

GenericButton Settings_Button(String const& name, Button const& defValue) {
  SettingsNode& node = GetSettings().GetNode(name);
  if (!node.value)
    node.value = SettingsEntry_Button(node.name, defValue);

  GenericButton g;
  node.value->GetValue((void*)&g);
  return g;
}

GenericColor Settings_Color(String const& name, Color const& defValue) {
  SettingsNode& node = GetSettings().GetNode(name);
  if (!node.value)
    node.value = SettingsEntry_Color(node.name, defValue);

  GenericColor g;
  node.value->GetValue((void*)&g);
  return g;
}

GenericFloat Settings_Float(
  String const& name,
  float minimum,
  float maximum,
  float defValue)
{
  SettingsNode& node = GetSettings().GetNode(name);
  if (!node.value)
    node.value = SettingsEntry_Float(node.name, defValue, minimum, maximum);

  GenericFloat g;
  node.value->GetValue((void*)&g);
  return g;
}

DefineFunction(Widget_Settings) {
  return Widget_Create1(new WidgetSettings(GetSettings().root));
}

GenericColor Settings_PrimaryColor() {
  static GenericColor g =
    Settings_Color("Interface/Primary Color", Color(0.1f, 0.5f, 1.0f));
  return g;
}

GenericColor Settings_SecondaryColor() {
  static GenericColor g =
    Settings_Color("Interface/Secondary Color", Color(1.0f, 0.3f, 0.2f));
  return g;
}
