#pragma once

#include <string>

#include "thc.h"

std::string calculate_move(std::string fen, int depth);
std::string rootmm(thc::ChessRules &cr, int depth);
double minimax(thc::ChessRules &cr, int depth);
double eval(thc::ChessRules &cr);
