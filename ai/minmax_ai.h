#ifndef MIN_MAX_AI_H__
#define MIN_MAX_AI_H__

#include "chess_ai.h"
#include "../core/Move.h"
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
 private:
  MoveScore MinMax(core::Position* position, int depth, int alpha, int beta, int move_index);
  
  int positions_evaluated;
  int max_depth;
  
  bool LegalMovesExist(core::Position* position, std::vector<core::Move> pseudolegal_moves);
  
  std::vector<core::Move> DeserializeMovegenList(std::string list, const core::Position& position) const;
  std::string SerializeMovegenList(std::vector<core::Move> list) const;
};

}

#endif
