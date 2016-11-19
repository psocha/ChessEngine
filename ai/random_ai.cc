#include "random_ai.h"
#include "../core/Move.h"
#include "../core/MoveGen.h"

#include <cstdlib>
#include <ctime>
#include <vector>
using core::Move;
using core::MoveGen;
using std::vector;
using std::string;

namespace ai {

RandomAI::RandomAI() : ChessAI() {
  srand(time(NULL));
}

RandomAI::~RandomAI() {}

std::string RandomAI::BestMove(core::Position position) {
  vector<Move> legal_moves = core::MoveGen::AllLegalMoves(position);
  int random_index = rand() % legal_moves.size();

  return legal_moves.at(random_index).ToString();
}

}
