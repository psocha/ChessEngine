#include "random_ai.h"

#include <cstdlib>
#include <ctime>
using std::set;
using std::string;

namespace ai {

RandomAI::RandomAI(core::Board *board) : ChessAI(board) {
  srand(time(NULL));
}

std::string RandomAI::BestMove() {
  set<string> legal_moves = board->GetLegalMoves();
  int random_index = rand() % legal_moves.size();

  set<string>::const_iterator it(legal_moves.begin());
  advance(it, random_index);

  return *it;
}

}
