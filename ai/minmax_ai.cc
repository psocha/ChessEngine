#include "minmax_ai.h"
#include "../core/move.h"
#include "../core/movegen.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

using core::Move;
using core::MoveGen;
using core::Position;
using std::cout;
using std::string;
using std::vector;

namespace ai {

MinMaxAI::MinMaxAI() : ChessAI() {
  max_depth = 5;
}

MinMaxAI::~MinMaxAI() {}

string MinMaxAI::BestMove(core::Position* position) {
  vector<Move> pseudolegal_moves = MoveGen::AllPseudolegalMoves(*position);
  
  this->positions_evaluated = 0;
  
  MoveScore result = MinMax(position, max_depth, 2 * BLACK_MAX, 2 * WHITE_MAX, -1);
                            
  int centipawn_evaluation = result.score;
  if (position->GetActiveColor() == core::BLACK) {
    centipawn_evaluation *= -1;
  }
  
  std::cout << "info nodes " << this->positions_evaluated << " score cp " << centipawn_evaluation << std::endl;
  return pseudolegal_moves.at(result.move_index).ToString();
}

MoveScore MinMaxAI::MinMax(core::Position* position, int depth, int alpha, int beta, int move_index) {  
  int best_index = 0;
  
  if (depth == 0) {
    if (position->IsCheck(position->GetActiveColor())) {
      vector<Move> pseudolegal_moves = MoveGen::AllPseudolegalMoves(*position);
      if (!LegalMovesExist(position, pseudolegal_moves)) {
        int score = position->GetActiveColor() == core::WHITE ?
          BLACK_MAX - (100 * depth) : WHITE_MAX + (100 * depth);
        return MoveScore(move_index == -1 ? 0 : move_index, score);
      }
    }
    
    int score = Evaluate(position);
    this->positions_evaluated++;
    return MoveScore(move_index == -1 ? 0 : move_index, score);
  }
  
  vector<Move> next_moves = MoveGen::AllPseudolegalMoves(*position);
  bool legal_move_found = false;
  
  for (unsigned int i = 0; i < next_moves.size(); i++) {
    Move move = next_moves.at(i);
    
    position->PerformMove(move.ToString());
    MoveScore next_score = MinMax(position, depth - 1, alpha, beta, i);
    position->UndoLastMove();
    
    if (position->GetActiveColor() == core::WHITE) {
      if (next_score.score > alpha) {
        if (MoveGen::IsPseudolegalMoveLegal(position, move)) {
          alpha = next_score.score;
          best_index = i;
          legal_move_found = true;
        }
      }
    } else {
      if (next_score.score < beta) {
        if (MoveGen::IsPseudolegalMoveLegal(position, move)) {
          beta = next_score.score;
          best_index = i;
          legal_move_found = true;
        }
      }
    }
    
    if (alpha >= beta) {
      break;
    }
  }
  
  if (!legal_move_found) {
    if (!LegalMovesExist(position, next_moves)) {
      if (position->IsCheck(position->GetActiveColor())) {
        int score = position->GetActiveColor() == core::WHITE ?
          BLACK_MAX - (100 * depth) : WHITE_MAX + (100 * depth);
        return MoveScore(move_index == -1 ? 0 : move_index, score);
      } else {
        return MoveScore(move_index == -1 ? 0 : move_index, 0.0);
      }
    }
  }

  return MoveScore(best_index, position->GetActiveColor() == core::WHITE ? alpha : beta);
}

void MinMaxAI::SetDepth(int depth) {
  max_depth = depth;
}

bool MinMaxAI::LegalMovesExist(Position* position, vector<Move> pseudolegal_moves) {
  for (Move move : pseudolegal_moves) {
    if (MoveGen::IsPseudolegalMoveLegal(position, move)) {
      return true;
    }
  }
  return false;
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
