#include "minmax_ai.h"
#include "../core/move.h"
#include "../core/movegen.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

using core::Move;
using core::MoveGen;
using core::Position;
using std::string;
using std::vector;

namespace ai {

MinMaxAI::MinMaxAI() : ChessAI() {
  max_depth = 3;
}

MinMaxAI::~MinMaxAI() {}

string MinMaxAI::BestMove(core::Position position) {
  vector<Move> moves = MoveGen::AllLegalMoves(position);
  
  if (moves.size() == 0) {
    throw NoMovesException();
  }
  
  this->positions_evaluated = 0;
  
  MoveScore result = MinMax(position, max_depth, -std::numeric_limits<double>::max(),
                            std::numeric_limits<double>::max(), -1);
                            
  int centipawn_evaluation = round(result.score * 100);
  if (position.GetActiveColor() == core::BLACK) {
    centipawn_evaluation *= -1;
  }
  
  std::cout << "info nodes " << this->positions_evaluated << " score cp " << centipawn_evaluation << std::endl;
  return moves.at(result.move_index).ToString();
}

MoveScore MinMaxAI::MinMax(core::Position position, int depth, double alpha, double beta, int move_index) {  
  int best_index = 0;
  
  if (depth == 0) {
    double score = Evaluate(position);
    this->positions_evaluated++;
    return MoveScore(move_index == -1 ? 0 : move_index, score);
  }
  
  vector<Move> next_moves = MoveGen::AllLegalMoves(position);
  
  if (next_moves.size() == 0) {
    double score = Evaluate(position);
    this->positions_evaluated++;
    return MoveScore(move_index == -1 ? 0 : move_index, score);
  }
  
  for (unsigned int i = 0; i < next_moves.size(); i++) {
    Move move = next_moves.at(i);
    Position next_position = position;
    
    next_position.PerformMove(move.ToString());
    MoveScore next_score = MinMax(next_position, depth - 1, alpha, beta, i);
    
    if (position.GetActiveColor() == core::WHITE) {
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
  
  return MoveScore(best_index, position.GetActiveColor() == core::WHITE ? alpha : beta);
}

void MinMaxAI::SetDepth(int depth) {
  max_depth = depth;
}

vector<Move> MinMaxAI::DeserializeMovegenList(string list, const Position& position) const {
  std::stringstream ss(list);
  vector<Move> move_list;

  while (ss.good()) {
    string move_string;
    getline(ss, move_string, ',');
    move_list.push_back(Move(move_string, position));
  }
  return move_list;
}

string MinMaxAI::SerializeMovegenList(vector<Move> list) const {
  string serialized_list = "";
  for (unsigned int i = 0; i < list.size(); i++) {
    serialized_list += list.at(i).ToString();
    if (i != list.size() - 1) {
      serialized_list += ",";
    }
  }
  return serialized_list;
}

}
