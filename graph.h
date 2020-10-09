#ifndef GRAPH_H
#define GRAPH_H

#include <cmath>
#include <cstdint>  // for platform independent types
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

typedef int32_t TVertexID;
typedef int32_t TEdgeID;

// forward class declarations
class CGraph;

typedef unordered_set<TEdgeID> TVertexEdgeList;

enum class EVertextColor : uint8_t { vtWHITE, vtBLUE, vtRED };

class CVertex {
   private:
    const CGraph& Owner;
    const TVertexID vertex_id;
    TVertexEdgeList edge_list;

   public:
    const TVertexID ID() const { return (vertex_id); }
    EVertextColor Color;

    const TVertexEdgeList& EdgeList() const { return (edge_list); }

    void AddEdge(const TEdgeID edge_id);
    void RebuildEdgeList();

    // constructor
    CVertex(const CGraph& g, const TVertexID id) : Owner(g), vertex_id(id) {}
    CVertex(const CVertex& v)
        : Owner(v.Owner), vertex_id(v.ID()), Color(v.Color) {}

    bool operator==(const CVertex& x) {
        return ((vertex_id == x.vertex_id) && (&Owner == &x.Owner));
    }
    bool operator<(const CVertex& x) {
        return ((vertex_id < x.vertex_id) && (&Owner == &x.Owner));
    }
    bool operator>(const CVertex& x) {
        return ((vertex_id > x.vertex_id) && (&Owner == &x.Owner));
    }
};

typedef vector<CVertex> TVertices;

class CEdge {
   private:
    const CGraph& Owner;
    const TEdgeID edge_id;

    const CVertex& from_vertex;
    const CVertex& to_vertex;
    float value_of_edge;

   public:
    const TEdgeID ID(void) const { return (edge_id); }
    const CVertex& From(void) const { return from_vertex; }
    const CVertex& To(void) const { return to_vertex; }
    const float Value(void) const { return value_of_edge; }
    void SetValue(const float val) { value_of_edge = val; }

    // constructor
    CEdge(const CGraph& g, const CVertex& source, const CVertex& dest,
          const TEdgeID id = -1, const float val = 0.0f)
        : Owner(g),
          from_vertex(source.ID() < dest.ID() ? source : dest),
          to_vertex(source.ID() < dest.ID() ? dest : source),
          edge_id(id),
          value_of_edge(val) {}

    // copier constructor
    CEdge(const CEdge& se)
        : Owner(se.Owner),
          from_vertex(se.from_vertex),
          to_vertex(se.to_vertex),
          edge_id(se.edge_id),
          value_of_edge(se.value_of_edge) {}

    // required for vector<CEdge>.erase()
    CEdge& operator=(CEdge&& x) { return x; }

    bool operator==(const CEdge& x);
};

typedef vector<CEdge> TEdges;

class CGraph {
   private:
    TVertices vertices;
    TEdges edges;

   public:
    CGraph() {}

    // this constructor creates graph randomly
    CGraph(const uint32_t vertex_count, const float pertange_of_edge_density);

    // creates graph from a file
    CGraph(const string filename);

    uint32_t NumberOfVertices(void) const { return vertices.size(); }
    uint32_t NumberOfEdges(void) const { return edges.size(); }

    const TVertices& VerticesList(void) const { return vertices; }
    const TEdges& EdgeList(void) const { return edges; }

    CVertex& GetVertex(const TVertexID id) { return (vertices[id]); }
    CEdge& GetEdge(const TEdgeID id) { return (edges[id]); }

    TVertexID AddVertex(const EVertextColor c = EVertextColor::vtWHITE);

    // adds new edge if edge is not there, otherwise just updates weight
    void AddEdge(CVertex& x, CVertex& y, const float edge_value);

    // removes the edge from x to y, if it is there.
    void RemoveEdge(CVertex& from, CVertex& to);
};

#endif