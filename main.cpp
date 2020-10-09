// Objective
// The objective of this assignment together with Homework 5  is to make a
// playable version of the game of HEX.Homework 4 is about drawing a HEX
// board and determining a legal position and a winning position.Homework 5
// will be to add an AI that can play HEX well.In Homework 5 we will use
// ideas from the video lectures that implement a Monte Carlo determined
// best move that will let your final program possibly exceed human ability.
//
// HW 5 expectations:
//   - The computer should be able to play Hex intelligently against a
//     human on an 11 by 11 board. (done)
//   - Efficiently determine within no more than 2 minutes what
//     the computer's move is. (done)
//
// Extra Comments from programmer:
//
//   - This C++ code is compiled in Visual Studio 2013
//   - Hex board will be represented by CGraph Class implemented previously
//   - Each dot on the board will correspond to a Vertex in the graph
//   - Edges between vertices will be calculated automatically as per HexBoard
//     game rules.
//   - Previous graph class extended to carry dot's coordinates and Colors
//   - Dijkstra Shortest Path algorithm is also extended to support colors!
//

// EXAMPLE OUTPUT:
// -----------------------------------------------------------------------
// D:\Dropbox\HexBoard\Release>HexBoard.exe
//
//   1 for Board size 7x7
//   2 for Board size 11x11
//   3 for Board size 13x13
//   4 for Board size 19x19
// Please choose one of boards (1, 2, 3 or 4): 2
//
//   1 for playing as X
//   2 for playing as O (plays first)
// Please choose which player you want to be (1 or 2): 2
//
//
//         O   O   O   O   O   O   O   O   O   O
//       A   B   C   D   E   F   G   H   I   J   K
//       ------------------------------------------
//     1 \ .   .   .   .   .   .   .   .   .   .   .\ 1
//   X                                                    X
//       2 \ .   .   .   .   .   .   .   .   .   .   .\ 2
//     X                                                    X
//         3 \ .   .   .   .   .   .   .   .   .   .   .\ 3
//       X                                                    X
//           4 \ .   .   .   .   .   .   .   .   .   .   .\ 4
//         X                                                    X
//             5 \ .   .   .   .   .   .   .   .   .   .   .\ 5
//           X                                                    X
//               6 \ .   .   .   .   .   .   .   .   .   .   .\ 6
//             X                                                    X
//                 7 \ .   .   .   .   .   .   .   .   .   .   .\ 7
//               X                                                    X
//                   8 \ .   .   .   .   .   .   .   .   .   .   .\ 8
//                 X                                                    X
//                     9 \ .   .   .   .   .   .   .   .   .   .   .\ 9
//                   X                                                    X
//                      10 \ .   .   .   .   .   .   .   .   .   .   .\ 10
//                     X                                                    X
//                        11 \ .   .   .   .   .   .   .   .   .   .   .\ 11
//                             ------------------------------------------
//                               A   B   C   D   E   F   G   H   I   J   K
//                                 O   O   O   O   O   O   O   O   O   O
// Human Player O move: d6
//
// Thinking...
// AI Player X move: D7
//
//
//         O   O   O   O   O   O   O   O   O   O
//       A   B   C   D   E   F   G   H   I   J   K
//       ------------------------------------------
//     1 \ .   .   .   .   .   .   .   .   .   .   .\ 1
//   X                                                    X
//       2 \ .   .   .   .   .   .   .   .   .   .   .\ 2
//     X                                                    X
//         3 \ .   .   .   .   .   .   .   .   .   .   .\ 3
//       X                                                    X
//           4 \ .   .   .   .   .   .   .   .   .   .   .\ 4
//         X                                                    X
//             5 \ .   .   .   .   .   .   .   .   .   .   .\ 5
//           X                                                    X
//               6 \ .   .   .   o   .   .   .   .   .   .   .\ 6
//             X                                                    X
//                 7 \ .   .   .   x   .   .   .   .   .   .   .\ 7
//               X                                                    X
//                   8 \ .   .   .   .   .   .   .   .   .   .   .\ 8
//                 X                                                    X
//                     9 \ .   .   .   .   .   .   .   .   .   .   .\ 9
//                   X                                                    X
//                      10 \ .   .   .   .   .   .   .   .   .   .   .\ 10
//                     X                                                    X
//                        11 \ .   .   .   .   .   .   .   .   .   .   .\ 11
//                             ------------------------------------------
//                               A   B   C   D   E   F   G   H   I   J   K
//                                 O   O   O   O   O   O   O   O   O   O
// Human Player O move:
// -----------------------------------------------------------------------

#include "hexboard.h"

int32_t ChooseBoardDimension() {
    int32_t result = 0;

    do {
        char choice;

        cout << "\n";
        cout << "   1 for Board size 7x7\n";
        cout << "   2 for Board size 11x11\n";
        cout << "   3 for Board size 13x13\n";
        cout << "   4 for Board size 19x19\n";
        cout << "Please choose one of boards (1, 2, 3 or 4): ";
        cin >> choice;

        switch (choice) {
            case '1':
                result = 7;
                break;

            case '2':
                result = 11;
                break;

            case '3':
                result = 13;
                break;

            case '4':
                result = 19;
                break;

            default:
                cout << "Invalid input\n\n";
        }
    } while (result == 0);

    return (result);
}

int main() {
    CHexBoard HexBoard(ChooseBoardDimension());
    HexBoard.Start();
}