#include "engine.h"

#include <cctype>
#include <iostream>
#include <unordered_map>
#include <vector>

std::unordered_map<char, int> piece_values = {
    {'p', 100},
    {'n', 350},
    {'b', 350},
    {'r', 525},
    {'q', 1000},
    {'k', 10000},
};

// TODO: delete this function when the engine is fully implemented
thc::ChessRules fen_to_cr(std::string fen) {
    // for testing
    thc::ChessRules cr;
    cr.Forsyth(fen.c_str());
    return cr;
}

void display_position(thc::ChessRules &cr) {
    std::string fen = cr.ForsythPublish();
    std::string s = cr.ToDebugStr();

    std::cout << s << std::endl;

    std::cout << fen << "\n"
              << std::endl;
}

// for bool color, white is true

std::string calculate_move(std::string fen) {
    thc::ChessRules cr;
    cr.Forsyth(fen.c_str());

    std::cout << cr.ToDebugStr() << std::endl;

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
        return eval(cr);
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

int eval(thc::ChessRules &cr) {
    int score;

    score = material_eval(cr);

    score = cr.WhiteToPlay() ? score : -score;
    return score;
}

int material_eval(thc::ChessRules &cr) {
    // calculates material for use in heuristic eval func, negative for black and positive for white
    // TODO: take other things into account (no pawns is worse, bishop pair is better)
    int material = 0;

    std::string pos = cr.ToDebugStr().substr(15, -1);

    if (cr.WhiteToPlay()) {
        for (int i = 0; i < pos.length(); i++) {
            if (isupper(pos[i]) && pos[i] != '.') {
                material += piece_values[tolower(pos[i])];
            }
        }
    } else {
        for (int i = 0; i < pos.length(); i++) {
            if (islower(pos[i]) && pos[i] != '.') {
                material += piece_values[tolower(pos[i])];
            }
        }
    }

    return material;
}}
