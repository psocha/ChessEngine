#include "random_ai.h"
#include "ai_util.h"
#include "../core/move.h"
#include "../core/movegen.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
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

std::string RandomAI::BestMove(core::Position* position) {
  vector<Move> legal_moves = core::MoveGen::AllLegalMoves(position);
  int random_index = rand() % legal_moves.size();

  int centipawn_evaluation = MaterialDifference(*position, 100, 300, 300, 500, 900);
  if (position->GetActiveColor() == core::BLACK) {
    centipawn_evaluation *= -1;
  }

  std::cout << "info nodes 1 score cp " << centipawn_evaluation << std::endl;

  return (legal_moves[random_index]).ToString();
}

}
