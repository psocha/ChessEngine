#include "minmax_ai.h"
#include "../core/move.h"
#include "../core/movegen.h"

#include <limits>
#include <vector>

using core::Move;
using core::MoveGen;
using core::Position;
using std::string;
using std::vector;

namespace ai {

MinMaxAI::MinMaxAI() : ChessAI() {}

string MinMaxAI::BestMove(core::Position position) {
  vector<Move> moves = MoveGen::AllLegalMoves(position);
  
  this->player_color = position.GetActiveColor();
  
  if (moves.size() == 0) {
    throw NoMovesException();
  }
  
  MoveScore result = MinMax(position, 4, std::numeric_limits<double>::min(),
                            std::numeric_limits<double>::max(), -1);
  return moves.at(result.move_index).ToString();
}

MoveScore MinMaxAI::MinMax(core::Position position, int depth, double alpha, double beta, int move_index) {
  vector<Move> next_moves = MoveGen::AllLegalMoves(position);
  
  int best_index = 0;
  
  if (next_moves.size() == 0 || depth == 0) {
    double score = Evaluate(position);
    return MoveScore(move_index == -1 ? 0 : move_index, score);
  } else {
    for (unsigned int i = 0; i < next_moves.size(); i++) {
      Move move = next_moves.at(i);
      Position next_position = position;
      
      next_position.PerformMove(move.ToString());
      MoveScore next_score = MinMax(next_position, depth - 1, alpha, beta, i);
      
      if (position.GetActiveColor() == this->player_color) {
        if (next_score.score > alpha) {
          alpha = next_score.score;
          best_index = i;
        }
      } else {
        if (next_score.score < beta) {
          beta = next_score.score;
          best_index = i;
        }
      }
      
      if (alpha >= beta) {
        break;
      }
      
    }
  }
  
  return MoveScore(best_index, position.GetActiveColor() == this->player_color ? alpha : beta);
}

}
