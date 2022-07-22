#include "Info.h"
#include "Detectable.h"

#include "Game/Object.h"

void ComponentInfo::Add(ObjectT* object, InfoLevelT level) {
  Pointer<ComponentDetectable> det = object->GetDetectable();
  if (!det)
    return;

  Time expiration = Universe_Age() + det->detectionTime; 
  ObjectID id = object->GetID();

  InfoEntry* entry = elements.get(id);
  if (entry) {
    entry->level = Max(entry->level, level);
    entry->expiration = Max(entry->expiration, expiration);
  } else {
   elements[id] = InfoEntry(level, expiration);
  }
}

InfoLevelT ComponentInfo::GetLevel(ObjectT* object) const {
  if (!object->GetDetectable())
    return InfoLevel_Max;

  InfoEntry* entry = (InfoEntry*)elements.get(object->GetID());
  if (!entry)
    return InfoLevel_None;

  /* If we have specific information about this object, then we need to
     check the timestamp, because specific information expires. */
  if (entry->level >= InfoLevel_Specific && entry->expiration < Universe_Age())
    entry->level = InfoLevel_Specific - 1;

  return entry->level;
}
