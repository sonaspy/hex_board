#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <chrono>
#include <cstdint>  // for platform independent types
#include <random>
#include <unordered_set>
using namespace std;
using namespace chrono;

#include "graph.h"  // our graph class
#include "shortestpath.h"

typedef vector<TVertexID> TVectorIDList;

class CHexBoard {
   private:
    // board dimension
    int32_t board_width_height;

    // random engine
    default_random_engine random_engine;

    // left-right and top-bottom vertices are used to find winner
    TVertexID left_vertex;
    TVertexID right_vertex;
    TVertexID top_vertex;
    TVertexID bottom_vertex;

    // player color
    EVertextColor human_player;
    EVertextColor ai_player;
    EVertextColor active_player;

    // this list is used to pick random places faster
    TVectorIDList unoccupied_vertices;

    // board structure as graph
    CGraph graph;

    // if there is a winner, contains the path
    CShortestPath shortest_path;

    void CreateHexBoardVertices(void);
    void CreateEdgesBetweenVertices(void);
    void CreateWinnerVerticesAndEdges(void);
    void PrintBoard(void);

    void ChoosePlayer(void);

    bool UserInputToVertextID(const string& in_str, TVertexID& id);
    string VertextIDToCoordStr(const TVertexID id);

    void NextPlayer(void);
    bool CheckForWinner(void);
    void DoMove(void);

    float DoMonteCarlo(int32_t id_inx, int32_t sim_count);
    TVertexID AI_MOVE(int32_t level = 1000);

    void OccupyVertex(const TVertexID v_id, const EVertextColor c);

   public:
    // hex board constructor
    CHexBoard(const int32_t board_width = 11)
        : board_width_height(board_width),
          human_player(EVertextColor::vtWHITE),
          ai_player(EVertextColor::vtWHITE),
          active_player(EVertextColor::vtWHITE),
          shortest_path(graph),
          random_engine(random_device{}()) {
        CreateHexBoardVertices();
        CreateEdgesBetweenVertices();

        // create virtual vertices to identify the winner
        CreateWinnerVerticesAndEdges();
    };

    ~CHexBoard(void);

    void Start(void);
};

#endif