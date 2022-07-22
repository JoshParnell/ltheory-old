#include "SphereTree.h"
#include "Mesh.h"

#if 0
namespace {
  SphereTree SphereTree_Create(Triangle const& triangle) {
    SphereTree tree = new SphereTreeT;
    tree->elements.push(triangle);
    tree->ComputeBound();
    return tree;
  }
}

SphereTree SphereTree_Create(Vector<Triangle> const& elements) {
  Vector<SphereTree> trees;
  for (size_t i = 0; i < elements.size(); ++i)
    trees.push(SphereTree_Create(elements[i]));
  return trees[0];
}

SphereTree SphereTree_Mesh(Mesh const& mesh) {
  Vector<Triangle> elements;
  for (size_t i = 0; i < mesh->indices.size(); i += 3)
    elements.push(Triangle(
      mesh->vertices[mesh->indices[i + 0]].p,
      mesh->vertices[mesh->indices[i + 1]].p,
      mesh->vertices[mesh->indices[i + 2]].p));
  return SphereTree_Create(elements);
}
#endif
