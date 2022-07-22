#ifndef LTE_Graph_h__
#define LTE_Graph_h__

#include "Array.h"
#include "Vector.h"

#include <algorithm>

namespace LTE {
  template <class T>
  struct Graph {
    struct Node {
      T data;
      Vector<uint> edges;

      Node(T const& data) :
        data(data)
        {}
    };

    struct Edge {
      uint src;
      uint dst;
      float cost;

      Edge(uint src,
           uint dst,
           float cost) :
        src(src),
        dst(dst),
        cost(cost)
        {}

      friend bool operator<(Edge const& one, Edge const& two) {
        return one.cost < two.cost;
      }
    };

    Vector<Node> nodes;
    Vector<Edge> edges;

    Graph& Add(T const& data) {
      nodes << Node(data);
      return *this;
    }

    Graph& Add(uint src, uint dst, float cost) {
      edges << Edge(src, dst, cost);
      return *this;
    }

    Graph* ComputeMST() const {
      Graph* mst = new Graph;
      mst->nodes = nodes;

      Vector<Edge> queue = edges;
      std::sort(queue.v.begin(), queue.v.end());

      struct SetArray {
        Array<int> setID;
        SetArray(uint nodeCount) : setID(nodeCount, -1) {}

        int& operator[](uint i) {
          return setID[i];
        }

        uint get(uint i) {
          return setID[i] == -1 ? i
                                : setID[i] = get(setID[i]);
        }
      } sets(nodes.size());

      for (uint i = 0; i < queue.size(); ++i) {
        uint i1 = sets.get(queue[i].src);
        uint i2 = sets.get(queue[i].dst);

        if (i1 != i2) {
          sets[i1] = i2;
          mst->Add(queue[i].src, queue[i].dst, queue[i].cost);
        }
      }

      return mst;
    }
  };
}

#endif
