#include "Projects.h"

#include "LTE/Function.h"

AutoClass(ProjectsIterator,
  Object, object,
  uint, index)
  ProjectsIterator() {}
};

FreeFunction(ProjectsIterator, Object_GetProjects,
  "Return an iterator to the projects managed by 'object'",
  Object, object)
{
  return ProjectsIterator(object, 0);
} FunctionAlias(Object_GetProjects, GetProjects);

FreeFunction(Project, ProjectsIterator_Access,
  "Return the contents of 'iterator'",
  ProjectsIterator, iterator)
{
  return iterator.object->GetProjects()->elements[iterator.index];
} FunctionAlias(ProjectsIterator_Access, Get);

VoidFreeFunction(ProjectsIterator_Advance,
  "Advance 'iterator'",
  ProjectsIterator, iterator)
{
  Mutable(iterator).index++;
} FunctionAlias(ProjectsIterator_Advance, Advance);

FreeFunction(bool, ProjectsIterator_HasMore,
  "Return whether 'iterator' has more elements",
  ProjectsIterator, iterator)
{
  return 
    iterator.object->GetProjects() &&
    iterator.index < iterator.object->GetProjects()->elements.size();
} FunctionAlias(ProjectsIterator_HasMore, HasMore);

FreeFunction(int, ProjectsIterator_Size,
  "Return the total number of elements in 'iterator'",
  ProjectsIterator, iterator)
{
  return (int)iterator.object->GetProjects()->elements.size();
} FunctionAlias(ProjectsIterator_Size, Size);
