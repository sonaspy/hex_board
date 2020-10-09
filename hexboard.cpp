#include "hexboard.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// convert vertex color to string
static char VertexColorToStr(const EVertextColor c) {
    char result = ' ';

    switch (c) {
        case EVertextColor::vtWHITE:
            result = '.';
            break;

        case EVertextColor::vtBLUE:
            result = 'o';
            break;

        case EVertextColor::vtRED:
            result = 'x';
            break;

        default:
            break;
    }

    return (result);
}

// operator overloading << for streams
static ostream& operator<<(ostream& stream, const EVertextColor c) {
    switch (c) {
        case EVertextColor::vtWHITE:
        case EVertextColor::vtBLUE:
        case EVertextColor::vtRED:
            stream << VertexColorToStr(c);
            break;

        default:
            stream << static_cast<int32_t>(c);
            break;
    }

    return (stream);
}

// destructor
CHexBoard::~CHexBoard() { cout << "\nBYE !.. \n"; }

void CHexBoard::CreateHexBoardVertices() {
    for (int32_t i = 0; i < board_width_height * board_width_height; i++) {
        TVertexID id = graph.AddVertex();
        unoccupied_vertices.push_back(id);
    }
}

void CHexBoard::CreateEdgesBetweenVertices() {
    for (uint32_t from = 0; from < graph.NumberOfVertices() - 1; from++) {
        for (uint32_t to = from + 1; to < graph.NumberOfVertices(); to++) {
            uint32_t fromX = from % board_width_height;
            uint32_t fromY = from / board_width_height;

            uint32_t toX = to % board_width_height;
            uint32_t toY = to / board_width_height;

            if (((fromX == toX) && (toY == fromY + 1)) ||
                ((fromY == toY) && (toX == fromX + 1)) ||
                ((toX == fromX - 1) && (toY == fromY + 1))) {
                graph.AddEdge(graph.GetVertex(from), graph.GetVertex(to), 1.0f);
            }
        }
    }
}

void CHexBoard::CreateWinnerVerticesAndEdges() {
    // if there is a path from left virtual vertex to right one
    // means, red is the winner
    left_vertex = graph.AddVertex(EVertextColor::vtRED);
    right_vertex = graph.AddVertex(EVertextColor::vtRED);

    // if there is a path from top virtual vertex to bottom one
    // means, blue player is the winner
    top_vertex = graph.AddVertex(EVertextColor::vtBLUE);
    bottom_vertex = graph.AddVertex(EVertextColor::vtBLUE);

    for (TVertexID i = 0; i < board_width_height; i++) {
        // from left to right
        graph.AddEdge(graph.GetVertex(left_vertex),
                      graph.GetVertex(i * board_width_height), 1.0f);
        graph.AddEdge(graph.GetVertex(right_vertex),
                      graph.GetVertex((i + 1) * board_width_height - 1), 1.0f);

        // from top to bottom
        graph.AddEdge(graph.GetVertex(top_vertex), graph.GetVertex(i), 1.0f);
        graph.AddEdge(
            graph.GetVertex(bottom_vertex),
            graph.GetVertex(i + (board_width_height - 1) * board_width_height),
            1.0f);
    }
}

void CHexBoard::OccupyVertex(const TVertexID v_id, const EVertextColor c) {
    graph.GetVertex(v_id).Color = c;

    // remove the occupied vertex from the list in order to
    // accomplish faster AI calculations
    unoccupied_vertices.erase(
        find(unoccupied_vertices.cbegin(), unoccupied_vertices.cend(), v_id));
}

// print hex board on the screen
void CHexBoard::PrintBoard() {
    string row_spacer = " ";

    cout << "\n\n";

    // draw top y-coordinates
    string ycoord = row_spacer + "      ";
    string yplayer = row_spacer + "        ";
    for (int32_t x = 0; x < board_width_height; x++) {
        char yc = 'A' + static_cast<char>(x);
        ycoord += yc;
        ycoord += "   ";

        if (x != board_width_height - 1) {
            yplayer += static_cast<char>(
                toupper(VertexColorToStr(EVertextColor::vtBLUE)));
            yplayer += "   ";
        }
    }
    cout << yplayer << "\n";
    cout << ycoord << "\n";
    // --------------------------------------------------------------------

    // draw top lines
    string under_line = row_spacer + "      ";
    for (int32_t x = 0; x < board_width_height; x++) {
        under_line += "----";
    }
    under_line.erase(under_line.end() - 2, under_line.end());

    cout << under_line << "\n";
    // --------------------------------------------------------------------

    // draw vertices with their x-coordinates
    for (int32_t y = 0; y < board_width_height; y++) {
        cout << row_spacer;
        cout << right << setw(5) << y + 1;
        cout << " \\ ";
        row_spacer += "  ";

        for (int32_t x = 0; x < board_width_height; x++) {
            CVertex& v = graph.GetVertex(y * board_width_height + x);

            // if there is a winner then draw path as capital
            if (find(shortest_path.ShortestPath.cbegin(),
                     shortest_path.ShortestPath.cend(),
                     &v) == shortest_path.ShortestPath.cend()) {
                cout << v.Color;
            } else {
                cout << static_cast<char>(toupper(VertexColorToStr(v.Color)));
            }

            if (x != (board_width_height - 1)) {
                cout << "   ";
            }
        }

        cout << "\\ " << left << setw(2) << y + 1 << "\n";

        if (y != board_width_height - 1) {
            cout << row_spacer
                 << static_cast<char>(
                        toupper(VertexColorToStr(EVertextColor::vtRED)));
            for (int32_t x = 0; x < board_width_height; x++) {
                cout << "    ";
            }
            cout << "        "
                 << static_cast<char>(
                        toupper(VertexColorToStr(EVertextColor::vtRED)));
            cout << "\n";
        }
    }
    // --------------------------------------------------------------------

    cout << row_spacer << '\b' << under_line << "\n";
    cout << row_spacer << " " << ycoord << "\n";
    cout << row_spacer << " " << yplayer << "\n";
}

void CHexBoard::ChoosePlayer() {
    human_player = EVertextColor::vtWHITE;
    ai_player = EVertextColor::vtWHITE;

    // blue always starts first (O)
    active_player = EVertextColor::vtBLUE;

    do {
        char choice;

        cout << "\n";
        cout << "   1 for playing as X\n";
        cout << "   2 for playing as O (plays first)\n";
        cout << "Please choose which player you want to be (1 or 2): ";
        cin >> choice;

        switch (choice) {
            case '1':
                human_player = EVertextColor::vtRED;
                ai_player = EVertextColor::vtBLUE;
                break;

            case '2':
                human_player = EVertextColor::vtBLUE;
                ai_player = EVertextColor::vtRED;
                break;

            default:
                cout << "Invalid input\n\n";
        }
    } while (human_player == EVertextColor::vtWHITE);

    NextPlayer();
}

// convert coordinates to vertex id
// A15
// 15J
// d1
// 9a - all are valid inputs depend on board size
bool CHexBoard::UserInputToVertextID(const string& in_str, TVertexID& id) {
    bool result = false;

    if ((in_str.length() > 1) && (in_str.length() < 4)) {
        int first = toupper(in_str.at(0)) - 'A';
        int last = toupper(*(in_str.end() - 1)) - 'A';
        const char* p = nullptr;
        int x = 0;

        if ((first >= 0) && (first < board_width_height)) {
            p = &in_str.c_str()[1];
            x = first;
        } else if ((last >= 0) && (last < board_width_height)) {
            p = in_str.c_str();
            x = last;
        }

        if (p != nullptr) {
            int y = atoi(p) - 1;  // invalid number returns -1
            if ((y >= 0) && (y < board_width_height)) {
                id = y * board_width_height + x;
                result = true;
            }
        }
    }

    return (result);
}

// converts vertex id to coordinates so that user can understand the position
// which AI plays
string CHexBoard::VertextIDToCoordStr(const TVertexID id) {
    char x = static_cast<char>((id % board_width_height) + 'A');
    int y = (id / board_width_height) + 1;

    string result = "";
    result += x;
    result += to_string(y);

    return (result);
}

// active player must be AI
float CHexBoard::DoMonteCarlo(int32_t id_inx, int32_t sim_count) {
    int32_t winner_count_active_player = 0;
    float result_rate = static_cast<float>(sim_count);
    EVertextColor tmp_active_player = active_player;

    TVectorIDList uovl;
    uovl.reserve(unoccupied_vertices.size() - 1);  // speed up

    do {
        // unoccupied vertices list
        for (int32_t i = 0; i != unoccupied_vertices.size(); i++) {
            if (i != id_inx) {
                uovl.push_back(unoccupied_vertices[i]);
            }
        }

        // vertex under test
        graph.GetVertex(unoccupied_vertices[id_inx]).Color = active_player;

        // shuffle unoccupied vertices
        shuffle(uovl.begin(), uovl.end(), random_engine);

        // fill all rest empty places randomly
        while (!uovl.empty()) {
            NextPlayer();

            // removing the end is faster and still random because of shuffle
            // above
            graph.GetVertex(uovl.back()).Color = active_player;
            uovl.pop_back();
        }

        // check for winner
        // checking after filling the board totally is ok too.
        // because there is no drawn in this game
        active_player = tmp_active_player;
        if (CheckForWinner()) {
            ++winner_count_active_player;
        }

        // pop back to the initial state
        for (TVertexID id : unoccupied_vertices) {
            graph.GetVertex(id).Color = EVertextColor::vtWHITE;
        }
    } while (--sim_count > 0);

    // bigger values are better
    result_rate = static_cast<float>(winner_count_active_player) / result_rate;
    return (result_rate);
}

// The program takes turns.It inputs the human(or machine opponent if playing
// against another program) move.When it is the AIs turn, it is to
// evaluate all legal available next moves and select a best move.Each legal
// move will be evaluated using  ~1000 or more trials.Each trial winds the game
// forward by randomly selecting successive moves until there is a winner.The
// trial is counted as a win or loss.The ratio : wins / trials are the AIs
// metric for picking which next move to make.
TVertexID CHexBoard::AI_MOVE(int32_t level) {
    float best_rate = -1.0;
    TVertexID best_move_id = unoccupied_vertices[0];

    for (int32_t id_inx = 0; id_inx != unoccupied_vertices.size(); id_inx++) {
        float rate = DoMonteCarlo(id_inx, level);

        if (rate > best_rate) {
            best_rate = rate;
            best_move_id = unoccupied_vertices[id_inx];
        }
    }

    return (best_move_id);
}

// switch to the next player
void CHexBoard::NextPlayer() {
    if (active_player == EVertextColor::vtRED) {
        active_player = EVertextColor::vtBLUE;
    } else {
        active_player = EVertextColor::vtRED;
    }
}

// take input from user or computer
void CHexBoard::DoMove() {
    TVertexID id;

    if (active_player == human_player) {
        bool valid_input = false;

        do {
            string str;
            cout << "Human Player "
                 << static_cast<char>(toupper(VertexColorToStr(active_player)))
                 << " move: ";
            cin >> str;
            if (UserInputToVertextID(str, id)) {
                // check whether selected place is empty
                valid_input =
                    (graph.GetVertex(id).Color == EVertextColor::vtWHITE);
                if (!valid_input) {
                    cout << "Selected place is already occupied!\n";
                }
            } else {
                cout << "Invalid input!\n";
            }
        } while (!valid_input);
    } else {
        cout << "\nThinking..." << endl;

        id = AI_MOVE(5000);
        cout << "AI Player "
             << static_cast<char>(toupper(VertexColorToStr(active_player)))
             << " move: " << VertextIDToCoordStr(id) << endl;
    }

    OccupyVertex(id, active_player);
}

// any winner?
bool CHexBoard::CheckForWinner() {
    // red player source and target vertices
    TVertexID source_vertex = left_vertex;
    TVertexID target_vertex = right_vertex;

    if (active_player == EVertextColor::vtBLUE) {
        source_vertex = top_vertex;
        target_vertex = bottom_vertex;
    }

    return (shortest_path.DijkstraShortestPath(source_vertex, target_vertex));
}

// starts the game
void CHexBoard::Start() {
    ChoosePlayer();

    do {
        // next player turn
        NextPlayer();

        // before human player action, print the board so that he can see the
        // latest status before his action
        if (active_player == human_player) {
            PrintBoard();
        }

        // does player move, AI or Human depend on active player
        DoMove();

    } while (
        !CheckForWinner());  // check whether there is a winner after last move

    PrintBoard();
    cout << "Player "
         << static_cast<char>(toupper(VertexColorToStr(active_player)))
         << " won the game!" << endl;
}