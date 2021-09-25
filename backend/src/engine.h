#pragma once

#include <string>

#include "thc.h"

thc::ChessRules fen_to_cr(std::string fen);
std::string calculate_move(std::string fen);
int rootnm(thc::ChessRules &cr, int depth);
int negamax(thc::ChessRules &cr, int depth);
int eval(thc::ChessRules &cr);
int material_eval(thc::ChessRules &cr);
