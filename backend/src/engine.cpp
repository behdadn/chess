#include "engine.h"

#include <iostream>
#include <vector>

#include "thc.h"

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
