#ifndef MOVEGEN_H__
#define MOVEGEN_H__

#include "move.h"
#include "position.h"

#include <set>

namespace core {

class MoveGen {
public:
  MoveGen();
  ~MoveGen();

  std::set<Move> AllLegalMoves(Position *position);

private:
  Position *position;

  std::set<Move> AllPseudolegalMoves();
};

}

#endif

