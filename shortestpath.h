#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include <cstdint>  // for platform independent types
#include <list>
using namespace std;

#include "graph.h"

typedef list<const CVertex*> TShortestPath;
typedef list<const CEdge*> TMinimalSpanningTree;

class CShortestPath {
   private:
    CGraph& graph;

   public:
    TShortestPath ShortestPath;
    float TotalDistance;
    bool TargetReached;

    float MinimalSpanningTreeDistance;
    TMinimalSpanningTree MinimalSpanningTree;

    // constructor
    CShortestPath(CGraph& g) : graph(g) {
        ShortestPath.clear();
        TotalDistance = 0.0f;
        TargetReached = false;

        MinimalSpanningTreeDistance = 0.0f;
        MinimalSpanningTree.clear();
    }

    bool DijkstraShortestPath(const TVertexID from_index,
                              const TVertexID to_index);
    void KruskalMinimalSpanningTree(void);
};

#endif