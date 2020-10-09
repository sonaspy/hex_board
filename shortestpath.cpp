#include "shortestpath.h"

#include <limits>
#include <queue>
#include <set>

// this is kind of linked list to store internal data required by shortest path
// algorithms
class CShortestPathData {
   public:
    float dist;
    bool visited;
    TVertexID previous;
    explicit CShortestPathData()
        : dist(numeric_limits<float>::infinity()),
          visited(false),
          previous(numeric_limits<int32_t>::min()) {}
};

// Minimum Heap Implementation using priority_queue
typedef pair<TVertexID, float> TMinHeapPair;
class CMinHeapPairComparator {
   public:
    // compares distances
    bool operator()(const TMinHeapPair& x, const TMinHeapPair& y) {
        return (x.second > y.second);
    }
};
typedef priority_queue<TMinHeapPair, vector<TMinHeapPair>,
                       CMinHeapPairComparator>
    TMinHeap;

// Dijkstra's_algorithm based on Pseudo code at wiki-pedia:
// http://en.wikipedia.org/wiki/Dijkstra's_algorithm
//
bool CShortestPath::DijkstraShortestPath(const TVertexID from_index,
                                         const TVertexID to_index) {
    TargetReached = false;
    ShortestPath.clear();
    TotalDistance = 0.0f;

    // source and target vertices color have to be same!
    if (graph.GetVertex(from_index).Color == graph.GetVertex(to_index).Color) {
        TMinHeap Q;
        vector<CShortestPathData> dijkstra_data(graph.NumberOfVertices());

        dijkstra_data[from_index].dist =
            0.0f;  // Distance from source to source
        Q.push(make_pair(from_index,
                         dijkstra_data[from_index]
                             .dist));  // Start off with just the source node

        while (!Q.empty() && !TargetReached) {
            // pop the min distance element
            TVertexID u_index = Q.top().first;
            CShortestPathData* u = &dijkstra_data[u_index];
            Q.pop();

            // mark this node as visited
            u->visited = true;

            // check whether we reached the target
            if (u_index == to_index) {
                // Now we can read the shortest path from source to target by
                // reverse iteration
                TargetReached = true;
                TotalDistance = u->dist;

                ShortestPath.push_front(
                    &graph.GetVertex(u_index));  // add target first
                while (u->previous >= 0) {
                    ShortestPath.push_front(&graph.GetVertex(u->previous));
                    u = &dijkstra_data[u->previous];
                }
            } else {
                for (const TEdgeID v : graph.GetVertex(u_index).EdgeList()) {
                    TVertexID v_inx = graph.GetEdge(v).To().ID();
                    if (v_inx == u_index) v_inx = graph.GetEdge(v).From().ID();

                    // if next vertex has same color with the source
                    // then we need to process its edges otherwise just ignore
                    if (graph.GetVertex(v_inx).Color ==
                        graph.GetVertex(from_index).Color) {
                        // accumulate shortest dist from source
                        float alt = u->dist + graph.GetEdge(v).Value();

                        if ((alt < dijkstra_data[v_inx].dist) &&
                            !dijkstra_data[v_inx].visited) {
                            // keep the shortest dist from source to v
                            dijkstra_data[v_inx].dist = alt;
                            dijkstra_data[v_inx].previous = u_index;

                            // Add unvisited v into the Q to be processed
                            Q.push(make_pair(v_inx, dijkstra_data[v_inx].dist));
                        }
                    }
                }  // end of for(TEdgeList::const_iterator..
            }
        }  // end of while(!Q.empty() && !reached_target..
    }

    return (TargetReached);
}

class CMinEdgePointerComparator {
   public:
    // compares distances
    bool operator()(const CEdge* x, const CEdge* y) {
        return (x->Value() > y->Value());
    }
};

// Kruskal minimal spanning tree algorithm based on Pseudo code at wiki-pedia:
// http://en.wikipedia.org/wiki/Kruskal's_algorithm
//
void CShortestPath::KruskalMinimalSpanningTree() {
    MinimalSpanningTree.clear();
    MinimalSpanningTreeDistance = 0.0f;

    priority_queue<const CEdge*, vector<const CEdge*>,
                   CMinEdgePointerComparator>
        Q;
    vector<TVertexID> visited_vertices;

    // add all edges to priority queue
    for (auto& e : graph.EdgeList()) {
        Q.push(&e);
    }

    // we should trace vertices for loops
    // initialize as non of vertices visited
    for (auto& v : graph.VerticesList()) {
        visited_vertices.push_back(v.ID());
    }

    while (!Q.empty()) {
        const CEdge* e = Q.top();
        Q.pop();

        TVertexID parent_from = visited_vertices[e->From().ID()];
        TVertexID parent_to = visited_vertices[e->To().ID()];

        if (parent_from != parent_to) {
            MinimalSpanningTree.push_back(e);
            MinimalSpanningTreeDistance += e->Value();

            // union(from, to)
            for (auto& visited_vertex : visited_vertices) {
                if (visited_vertex == parent_to) {
                    visited_vertex = parent_from;
                }
            }
        }
    }
}
