#pragma once

#include <string>

#include "thc.h"

thc::ChessRules fen_to_cr(std::string fen);
std::string calculate_move(std::string fen, int depth);
int rootmm(thc::ChessRules &cr, int depth);
double minimax(thc::ChessRules &cr, int depth);
double eval(thc::ChessRules &cr);
