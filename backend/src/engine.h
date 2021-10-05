#pragma once

#include <string>

#include "thc.h"

thc::ChessRules fen_to_cr(std::string fen);
std::string calculate_move(std::string fen, int depth);
int rootmm(thc::ChessRules &cr, int depth);
int minimax(thc::ChessRules &cr, int depth);
int eval(thc::ChessRules &cr);
int material_eval(thc::ChessRules &cr);
double psqt_eval(thc::ChessRules &cr);
