#ifndef MIN_MAX_AI_H__
#define MIN_MAX_AI_H__

#include "chess_ai.h"
#include "../core/move.h"
#include "../core/position.h"

#include <string>
#include <vector>

namespace ai {

const int WHITE_MAX =  100000;
const int BLACK_MAX = -100000;

struct MoveScore {
  int move_index;
  int score;

  MoveScore(int index, int score) {
    this->move_index = index;
    this->score = score;
  }
};

class MinMaxAI : public ChessAI {
 public:
  MinMaxAI();
  virtual ~MinMaxAI();

  virtual std::string BestMove(core::Position* position);

  virtual int Evaluate(core::Position* position) = 0;

  void SetDepth(int depth);
  void SetSuppressLogs(bool suppress);
 private:
  MoveScore MinMax(core::Position* position, int depth, int alpha, int beta, int move_index);

  int positions_evaluated;
  int max_depth;
  bool suppress_logs;

  bool LegalMovesExist(core::Position* position, const std::vector<core::Move>& moves, bool guaranteed_legal_moves);
};

}

#endif
