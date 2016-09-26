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

  std::set<Move> AllLegalMoves(Position position);

private:
  Position position;

  std::set<Move> AllPseudolegalMoves();
  
  std::set<Move> GetPawnMoves(Square square);
  std::set<Move> GetKnightMoves(Square square);
  std::set<Move> GetDiagonalMoves(Square square, bool limit_to_one);
  std::set<Move> GetOrthogonalMoves(Square square, bool limit_to_one);
};

}

#endif

