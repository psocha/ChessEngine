#ifndef MIN_MAX_AI_H__
#define MIN_MAX_AI_H__

#include "chess_ai.h"
#include "../core/Move.h"
#include "../core/position.h"

#include <string>

namespace ai {
  
class NoMovesException {};

const double WHITE_MAX = 1000;
const double BLACK_MAX = -1000;

struct MoveScore {
  int move_index;
  double score;
  
  MoveScore(int index, double score) {
    this->move_index = index;
    this->score = score;
  }
};

class MinMaxAI : public ChessAI {
 public:
  MinMaxAI();
  virtual ~MinMaxAI();

  virtual std::string BestMove(core::Position position);
  
  virtual double Evaluate(core::Position position) = 0;
  
  void SetDepth(int depth);
 private:
  MoveScore MinMax(core::Position position, int depth, double alpha, double beta, int move_index);
  
  int positions_evaluated;
  int max_depth;
  
  std::vector<core::Move> DeserializeMovegenList(std::string list, const core::Position& position) const;
  std::string SerializeMovegenList(std::vector<core::Move> list) const;
};

}

#endif
