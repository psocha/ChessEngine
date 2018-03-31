#include "minmax_ai.h"
#include "../core/move.h"
#include "../core/movegen.h"

#include <iostream>
#include <vector>

using core::Move;
using core::MoveGen;
using core::Position;
using core::WHITE;
using core::BLACK;
using std::cout;
using std::string;
using std::vector;

namespace ai {

MinMaxAI::MinMaxAI() : ChessAI() {
  max_depth = 5;
  suppress_logs = false;
}

MinMaxAI::~MinMaxAI() {}

string MinMaxAI::BestMove(Position* position) {
  vector<Move> legal_moves = MoveGen::AllLegalMoves(position);

  if (legal_moves.size() == 1) {
    return legal_moves[0].ToString();
  }

  this->positions_evaluated = 0;

  MoveScore result = MinMax(position, max_depth, 2 * BLACK_MAX, 2 * WHITE_MAX, -1);

  int centipawn_evaluation = result.score;
  if (position->GetActiveColor() == BLACK) {
    centipawn_evaluation *= -1;
  }

  if (!suppress_logs) {
    std::cout << "info nodes " << this->positions_evaluated << " score cp " << centipawn_evaluation << std::endl;
  }

  return legal_moves[result.move_index].ToString();
}

MoveScore MinMaxAI::MinMax(Position* position, int depth, int alpha, int beta, int move_index) {
  int best_index = 0;

  if (depth == 0) {
    if (position->IsCheck(position->GetActiveColor())) {
      vector<Move> pseudolegal_moves = MoveGen::AllPseudolegalMoves(*position);
      if (!LegalMovesExist(position, pseudolegal_moves)) {
        int score = position->GetActiveColor() == WHITE ? BLACK_MAX : WHITE_MAX;
        return MoveScore(move_index == -1 ? 0 : move_index, score);
      }
    }

    int score;
    if (position->IsThreeFold()) {
      score = 0;
    } else {
      score = Evaluate(position);
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
    Move move = next_moves[i];

    position->PerformMove(move.ToString());
    MoveScore next_score = MinMax(position, depth - 1, alpha, beta, i);
    position->UndoLastMove();

    if (position->GetActiveColor() == WHITE) {
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
        int score = position->GetActiveColor() == WHITE ?
          BLACK_MAX - (100 * depth) : WHITE_MAX + (100 * depth);
        return MoveScore(move_index == -1 ? 0 : move_index, score);
      } else {
        return MoveScore(move_index == -1 ? 0 : move_index, 0);
      }
    }
  }

  return MoveScore(best_index, position->GetActiveColor() == WHITE ? alpha : beta);
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
