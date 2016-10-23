#include "random_ai.h"
#include "../core/Move.h"
#include "../core/MoveGen.h"

#include <cstdlib>
#include <ctime>
#include <set>
using core::Move;
using core::MoveGen;
using std::set;
using std::string;

namespace ai {

RandomAI::RandomAI() : ChessAI() {
  srand(time(NULL));
}

std::string RandomAI::BestMove(core::Position position) {
  set<Move> legal_moves = core::MoveGen::AllLegalMoves(position);
  int random_index = rand() % legal_moves.size();

  set<Move>::const_iterator it(legal_moves.begin());
  advance(it, random_index);

  return (*it).ToString();
}

}
