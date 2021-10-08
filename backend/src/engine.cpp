#include "engine.h"

#include <boost/algorithm/string.hpp>
#include <cctype>
#include <iostream>
#include <unordered_map>
#include <vector>

std::unordered_map<char, int> piece_values = {
    // used for material eval
    {'p', 100},
    {'n', 350},
    {'b', 350},
    {'r', 525},
    {'q', 1000},
    {'k', 10000},
};

std::unordered_map<char, int> psqt_index = {
    // used for fast traversal of the psqt array
    {'K', 0},
    {'k', 1},
    {'Q', 2},
    {'q', 3},
    {'R', 4},
    {'r', 5},
    {'B', 6},
    {'b', 7},
    {'N', 8},
    {'n', 9},
    {'P', 10},
    {'p', 11},
};

double psqt[12][8][8] = {
    // used for the value to use in the piece square table eval
    {
        // white king
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
        {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
        {2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0},
        {2.0, 3.0, 1.0, 0.0, 0.0, 1.0, 3.0, 2.0},
    },
    {
        // black king
        {2.0, 3.0, 1.0, 0.0, 0.0, 1.0, 3.0, 2.0},
        {2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0},
        {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
        {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    },
    {
        // white queen
        {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
        {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
        {-1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
        {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
        {0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
        {-1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
        {-1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, -1.0},
        {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
    },
    {
        // black queen
        {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
        {-1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, -1.0},
        {-1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
        {0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
        {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
        {-1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
        {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
        {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
    },
    {
        // white rook
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
    },
    {
        // black rook
        {0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
        {0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        // white bishop
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
        {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
        {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
        {-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},
        {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, -1.0},
        {-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0},
        {-1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -1.0},
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
    },
    {
        // black bishop
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
        {-1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -1.0},
        {-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0},
        {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, -1.0},
        {-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},
        {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
        {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
    },
    {
        // white knight
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
        {-4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0},
        {-3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0, -3.0},
        {-3.0, 0.5, 1.0, 2.0, 2.0, 1.5, 0.5, -3.0},
        {-3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0, -3.0},
        {-3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -3.0},
        {-4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0},
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
    },
    {
        // black knight
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
        {-4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0},
        {-3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -3.0},
        {-3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0, -3.0},
        {-3.0, 0.5, 1.0, 2.0, 2.0, 1.5, 0.5, -3.0},
        {-3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0, -3.0},
        {-4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0},
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
    },
    {
        // white pawn
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
        {0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
        {0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
        {0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5},
        {0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        // black pawn
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
        {0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5},
        {0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
        {0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
        {1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    },
};

std::string calculate_move(std::string fen, int depth) {
    // function that takes the fen as input and returns the computer move
    // depth tracks how deep the algorithm is meant to move in the tree of possible moves (depth = no. of moves to look ahead)

    thc::ChessRules cr;
    cr.Forsyth(fen.c_str());
    // creates new ChessRules instance, which represents the game, and sets it up using the fen input

    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves);
    // generates the list of moves that the computer can legally play

    thc::Move mv = moves[rootmm(cr, depth)];
    // finds the move that the algorithm has calculated as the best

    return mv.NaturalOut(&cr);
    // returns that move as a string for the server to send back to the site
}

int rootmm(thc::ChessRules &cr, int depth) {
    // the root minimax function that starts the evaluation process
    // returns the index of the calculated best move in the moves vector

    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves);
    // generates list of legal moves

    thc::ChessRules temp_cr;
    // a temporary ChessRules instance for making the moves and evaluating the positions

    double score = -99999;
    // set very low so technically all moves are better at first
    double temp_score;
    // variable used to compare the temporary score to the current score
    int index;
    // variable that holds the index of the "best" move

    for (int i = 0; i < moves.size(); i++) {
        // iterates and plays through all the first moves
        temp_cr = cr;
        // sets the temp_cr equal to the current game

        temp_cr.PlayMove(moves[i]);
        // plays a move in the temp_cr which is reset everytime the loop iterates
        temp_score = minimax(temp_cr, depth - 1);
        // calls the real minimax algorithm on the position after that move is played in the temp_Cr

        if (temp_score > score) {
            // if the score returned by the minimax is higher than the current score
            score = temp_score;
            // sets score to the score of the position to remember the best eval achieved
            index = i;
            // store the index of that "optimal" move
        }
    }
    return index;
    // returns index of the calculated move
}

double minimax(thc::ChessRules &cr, int depth) {
    // moves through the tree of possible moves and evaluates the positions
    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves);
    // generates list of legal moves

    if (depth == 0 || moves.size() == 0) {
        // if there are no more moves(checkmate, draw) or the depth is 0, return the evaluation of the current position
        return eval(cr);
    }

    // same as rootmm function
    int max = -99999;
    int score;
    thc::ChessRules temp_cr;
    for (int i = 0; i < moves.size(); i++) {
        temp_cr = cr;
        temp_cr.PlayMove(moves[i]);
        score = -minimax(temp_cr, depth - 1);
        if (score > max) {
            max = score;
        }
    }
    return max;
}

double eval(thc::ChessRules &cr) {
    // the function that actually evaluates the positions and returns the score of each one
    int material = 0;
    double psqt_score = 0;
    // variables to keep track of material score and piece square table score

    std::string pos_str = cr.ToDebugStr().substr(15, -1);
    // gets the position represented by a string

    std::vector<std::string> pos;
    boost::algorithm::split(pos, pos_str, boost::algorithm::is_any_of("\n"));
    // splits up the position string into an array of strings, each of which represents a row of the board

    if (cr.WhiteToPlay()) {
        // the loop if white is playing
        for (int i = 0; i < 8; i++) {
            // iterate through the rows
            for (int j = 0; j < 8; j++) {
                // iterate through the squares of each row
                if (isupper(pos[i][j])) {
                    // if there is a white piece on that square
                    psqt_score += psqt[psqt_index[pos[i][j]]][i][j];
                    // add it's piece-square table score to the sum
                    material += piece_values[tolower(pos[i][j])];
                    // add the value of that piece to the material count
                }
            }
        }
    } else {
        // the loop if black is playing
        for (int i = 0; i < 8; i++) {
            // iterate through the rows
            for (int j = 0; j < 8; j++) {
                // iterate through the squares of each row
                if (islower(pos[i][j])) {
                    // if there is a black piece on that square
                    psqt_score += psqt[psqt_index[pos[i][j]]][i][j];
                    // add it's piece-square table score to the sum
                    material += piece_values[tolower(pos[i][j])];
                    // add the value of that piece to the material count
                }
            }
        }
    }

    return 0.0001 * material * psqt_score;
    // returns the product of 0.0001 x the material and the piece-square table score
}