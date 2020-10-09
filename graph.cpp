#include "graph.h"

#include <chrono>
#include <fstream>
#include <random>
#include <sstream>

using namespace chrono;

///////////////////////////////////////////////////////////////////////////
void CVertex::AddEdge(const TEdgeID id) { edge_list.insert(id); }

void CVertex::RebuildEdgeList() {
    edge_list.clear();

    for (TEdges::const_iterator edge = Owner.EdgeList().cbegin();
         edge != Owner.EdgeList().cend(); edge++) {
        if ((edge->From().ID() == vertex_id) ||
            (edge->To().ID() == vertex_id)) {
            AddEdge(edge->ID());
        }
    }
}

///////////////////////////////////////////////////////////////////////////

bool CEdge::operator==(const CEdge& x) {
    return ((&x.Owner == &Owner) && (x.From().ID() == from_vertex.ID()) &&
            (x.To().ID() == to_vertex.ID()));
}

///////////////////////////////////////////////////////////////////////////

void CGraph::AddEdge(CVertex& from, CVertex& to, const float edge_value) {
    CEdge le(*this, from, to, edges.size(), edge_value);
    TEdges::iterator it = find(edges.begin(), edges.end(), le);

    // if it is not available in graph.edgelist
    if (it == edges.end()) {
        // add to graph edge list
        edges.push_back(le);

        from.AddEdge(le.ID());
        to.AddEdge(le.ID());
    } else {
        // edge already exists, then just update the weight
        it->SetValue(edge_value);
    }
}

void CGraph::RemoveEdge(CVertex& from, CVertex& to) {
    CEdge le(*this, from, to);
    TEdges::iterator it = find(edges.begin(), edges.end(), le);

    if (it != edges.end()) {
        edges.erase(it);

        from.RebuildEdgeList();
        to.RebuildEdgeList();
    }
}

CGraph::CGraph(const uint32_t vertex_count,
               const float pertange_of_edge_density) {
    // take seed from system timer in order to generate random numbers
    default_random_engine generator(
        static_cast<uint32_t>(system_clock::now().time_since_epoch().count()));

    uniform_real_distribution<float> weight_distribution(1.0f, 10.0f);
    uniform_real_distribution<float> density_distribution(0.0f, 1.0f);

    // create vertices first at required amount
    for (uint32_t vertex_index = 0; vertex_index < vertex_count;
         vertex_index++) {
        (void)AddVertex();
    }

    // create edge connectivity between vertices at desired density
    // and each edge weight should be between 1.0 and 10.0
    // ok, lets do..
    //
    // from all possible source vertices
    for (uint32_t source = 0; source < vertex_count - 1; source++) {
        // to all possible destination vertices
        for (uint32_t dest = source + 1; dest < vertex_count; dest++) {
            // if random value in specified range
            if (density_distribution(generator) <= pertange_of_edge_density) {
                // then add edge between two vertices
                AddEdge(GetVertex(source), GetVertex(dest),
                        weight_distribution(generator));
            }
        }
    }
}

// example file
// 20                - vertex count
// 0 1 17            - from_vertex  to_vertex  distance
// 0 2 2             - from_vertex  to_vertex  distance
// 0 3 9             - from_vertex  to_vertex  distance
// 0 4 24            - from_vertex  to_vertex  distance
// ..
// ..
CGraph::CGraph(const string filename) {
    ifstream txt_stream_file(filename);
    string line = "";
    bool first_line = true;
    uint32_t vertex_count = 0;

    // if file is there and successfully opened
    if (txt_stream_file.is_open()) {
        // read text file line by line
        while (getline(txt_stream_file, line)) {
            istringstream string_stream(line);

            // first line indicates vertex count
            if (first_line) {
                first_line = false;

                string_stream >> vertex_count;

                for (uint32_t i = 0; i < vertex_count; i++) {
                    (void)AddVertex();
                }
            } else {
                // edge definitions
                uint32_t from_vertex_index;
                uint32_t to_vertex_index;
                float edge_cost;
                string_stream >> from_vertex_index >> to_vertex_index >>
                    edge_cost;

                AddEdge(GetVertex(from_vertex_index),
                        GetVertex(to_vertex_index), edge_cost);
            }
        }

        // close the file
        txt_stream_file.close();
    } else {
        perror("\nFile Error ");
    }
}

TVertexID CGraph::AddVertex(const EVertextColor c) {
    CVertex lv(*this, vertices.size());
    lv.Color = c;

    vertices.push_back(lv);
    return (lv.ID());
}