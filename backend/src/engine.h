#include <string>

#include "thc.h"

std::string eval(std::string fen);
int rootnm(thc::ChessRules &cr, int depth);
int negamax(thc::ChessRules &cr, int depth);
int heuristic_eval(thc::ChessRules &cr);
int material_eval(thc::ChessRules &cr);
thc::ChessRules fen_to_cr(std::string fen);