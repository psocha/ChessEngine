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
  
  double material_difference = MaterialDifference(*position, 1.0, 3.0, 3.0, 5.0, 9.0);
  int centipawn_evaluation = round(material_difference * 100);
  if (position->GetActiveColor() == core::BLACK) {
    centipawn_evaluation *= -1;
  }
  
  std::cout << "info nodes 1 score cp " << centipawn_evaluation << std::endl;

  return legal_moves.at(random_index).ToString();
}

}
