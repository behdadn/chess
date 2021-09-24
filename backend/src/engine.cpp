#include "engine.h"

#include <iostream>
#include <unordered_map>
#include <vector>

#include "thc.h"

std::unordered_map<char, int> centipawn = {
    {'p', 100},
    {'n', 350},
    {'b', 350},
    {'r', 525},
    {'q', 1000},
    {'k', 10000},
};

void display_position(thc::ChessRules &cr) {
    std::string fen = cr.ForsythPublish();
    std::string s = cr.ToDebugStr();

    std::cout << s.c_str() << std::endl;

    std::cout << fen.c_str() << "\n"
              << std::endl;
}

std::string eval(std::string fen_input) {
    thc::ChessRules cr;
    cr.Forsyth(fen_input.c_str());

    std::vector<thc::Move> moves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    cr.GenLegalMoveList(moves, check, mate, stalemate);

    std::cout << check.size() << std::endl;

    thc::Move mv = moves[rand() % (moves.size() + 1)];
    cr.PlayMove(mv);

    display_position(cr);

    return cr.ForsythPublish();
}

// calculates material for use in heuristic eval func, negative for black and positive for white
// also includes following factors:
// + bishop pair
// - no pawns
int material(thc::ChessRules &cr) {
    int material_sum = 0;

    return material_sum;
}