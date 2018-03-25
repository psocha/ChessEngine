#include "minmax_ai.h"
#include "../core/move.h"
#include "../core/movegen.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using core::Move;
using core::MoveGen;
using core::Position;
using std::cout;
using std::string;
using std::unordered_map;
using std::vector;

namespace ai {

MinMaxAI::MinMaxAI() : ChessAI() {
  max_depth = 5;
  evaluation_cache = new unordered_map<string, int>();
  suppress_logs = false;
}

MinMaxAI::~MinMaxAI() {
  delete evaluation_cache;
}

string MinMaxAI::BestMove(core::Position* position) {
  vector<Move> legal_moves = MoveGen::AllLegalMoves(position);

  this->positions_evaluated = 0;

  if (evaluation_cache->size() > EVAL_CACHE_MAX_SIZE) {
    evaluation_cache->clear();
  }

  MoveScore result = MinMax(position, max_depth, 2 * BLACK_MAX, 2 * WHITE_MAX, -1);

  int centipawn_evaluation = result.score;
  if (position->GetActiveColor() == core::BLACK) {
    centipawn_evaluation *= -1;
  }

  if (!suppress_logs) {
    std::cout << "info nodes " << this->positions_evaluated << " score cp " << centipawn_evaluation << std::endl;
  }

  return legal_moves.at(result.move_index).ToString();
}

MoveScore MinMaxAI::MinMax(core::Position* position, int depth, int alpha, int beta, int move_index) {
  int best_index = 0;

  string serialized_position = position->Serialize();

  if (depth == 0) {
    if (position->IsCheck(position->GetActiveColor())) {
      vector<Move> pseudolegal_moves = MoveGen::AllPseudolegalMoves(*position);
      if (!LegalMovesExist(position, pseudolegal_moves)) {
        int score = position->GetActiveColor() == core::WHITE ?
          BLACK_MAX - (100 * depth) : WHITE_MAX + (100 * depth);
        (*evaluation_cache)[serialized_position] = score;
        return MoveScore(move_index == -1 ? 0 : move_index, score);
      }
    }

    int score;
    if (evaluation_cache->count(serialized_position) > 0) {
      score = evaluation_cache->at(serialized_position);
    } else {
      score = Evaluate(position);
      (*evaluation_cache)[serialized_position] = score;
    }
    this->positions_evaluated++;

    return MoveScore(move_index == -1 ? 0 : move_index, score);
  }

  vector<Move> next_moves;
  if (depth == this->max_depth) {
    next_moves = MoveGen::AllLegalMoves(position);
  } else {
    next_moves = MoveGen::AllPseudolegalMoves(*position);
  }

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
        (*evaluation_cache)[serialized_position] = score;
        return MoveScore(move_index == -1 ? 0 : move_index, score);
      } else {
        (*evaluation_cache)[serialized_position] = 0;
        return MoveScore(move_index == -1 ? 0 : move_index, 0);
      }
    }
  }

  return MoveScore(best_index, position->GetActiveColor() == core::WHITE ? alpha : beta);
}

void MinMaxAI::SetDepth(int depth) {
  max_depth = depth;
}

void MinMaxAI::SetSuppressLogs(bool suppress) {
  suppress_logs = suppress;
}

bool MinMaxAI::LegalMovesExist(Position* position, vector<Move> pseudolegal_moves) {
  for (Move move : pseudolegal_moves) {
    if (MoveGen::IsPseudolegalMoveLegal(position, move)) {
      return true;
    }
  }
  return false;
}

}
