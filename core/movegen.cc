#include "movegen.h"
using std::set;

namespace core {

MoveGen::MoveGen() {}

MoveGen::~MoveGen() {}

set<Move> MoveGen::AllLegalMoves(Position *position) {
  set<Move> legal_moves;

  return legal_moves;
}

set<Move> MoveGen::AllPseudolegalMoves() {
  set<Move> pseudolegal_moves;

  return pseudolegal_moves;
}

}

