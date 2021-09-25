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

bool COLOR;

void set_color(thc::ChessRules &cr) { COLOR = cr.WhiteToPlay(); }

void display_position(thc::ChessRules &cr) {
    std::string fen = cr.ForsythPublish();
    std::string s = cr.ToDebugStr();

    std::cout << s.c_str() << std::endl;

    std::cout << fen.c_str() << "\n"
              << std::endl;
}

// for bool color, white is true

std::string eval(std::string fen) {
    thc::ChessRules cr;
    cr.Forsyth(fen.c_str());

    set_color(cr);

    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves);
    // std::vector<bool> check;
    // std::vector<bool> mate;
    // std::vector<bool> stalemate;
    // cr.GenLegalMoveList(moves, check, mate, stalemate);

    // std::cout << check.size() << std::endl;

    // thc::Move mv = moves[rand() % (moves.size() + 1)];
    // cr.PlayMove(mv);

    cr.PlayMove(moves[rootnm(cr, 3)]);

    display_position(cr);

    return cr.ForsythPublish();
}

int rootnm(thc::ChessRules &cr, int depth) {
    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves);

    thc::ChessRules temp_cr;

    int score = -99999;
    int temp_score;
    int index;

    for (int i = 0; i < moves.size(); i++) {
        temp_cr = cr;

        temp_cr.PlayMove(moves[i]);
        temp_score = negamax(temp_cr, depth - 1);

        if (temp_score > score) {
            score = temp_score;
            index = i;
        }
    }

    return index;
}

int negamax(thc::ChessRules &cr, int depth) {
    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves);
    if (depth == 0 || moves.size() == 0) {
        return heuristic_eval(cr);
    }
    int max = -99999;
    int score;
    thc::ChessRules temp_cr;
    for (int i = 0; i < moves.size(); i++) {
        temp_cr = cr;
        temp_cr.PlayMove(moves[i]);
        score = -negamax(temp_cr, depth - 1);
        if (score > max) {
            max = score;
        }
    }
    return max;
}

int heuristic_eval(thc::ChessRules &cr) {
    int score;

    score = COLOR ? score : -score;
    return score;
}

// calculates material for use in heuristic eval func, negative for black and positive for white
// also includes following factors:
// + bishop pair
// - no pawns
int material_eval(thc::ChessRules &cr) {
    int material = 0;

    return material;
}