#include "movegen.h"

#include <algorithm>
using std::set;

namespace core {

MoveGen::MoveGen() {}

MoveGen::~MoveGen() {}

set<Move> MoveGen::AllLegalMoves(Position position) {
  this->position = position;
  set<Move> legal_moves = AllPseudolegalMoves();

  return legal_moves;
}

set<Move> MoveGen::AllPseudolegalMoves() {
  set<Move> pseudolegal_moves;
  Color color = position.GetActiveColor();
  
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Square square = Square(rank, file);
      SquareContents contents = position.ContentsAt(square);
      if (ColorOfContents(contents) == color) {
        set<Move> square_moves;
        
        PieceType piece_type = GetPieceType(contents);
        if (piece_type == PAWN) {
          square_moves = GetPawnMoves(square);
        } else if (piece_type == KNIGHT) {
          square_moves = GetKnightMoves(square);
        } else if (piece_type == BISHOP) {
          square_moves = GetDiagonalMoves(square, false);
        } else if (piece_type == ROOK) {
          square_moves = GetOrthogonalMoves(square, false);
        } else if (piece_type == QUEEN) {
          set<Move> diagonal_moves = GetDiagonalMoves(square, false);
          set<Move> orthogonal_moves = GetOrthogonalMoves(square, false);
          //std::merge(diagonal_moves.begin(), diagonal_moves.end(),
            //orthogonal_moves.begin(), orthogonal_moves.end(), square_moves.begin());
        } else if (piece_type == KING) {
          set<Move> diagonal_moves = GetDiagonalMoves(square, true);
          set<Move> orthogonal_moves = GetOrthogonalMoves(square, true);
          //std::merge(diagonal_moves.begin(), diagonal_moves.end(),
            //orthogonal_moves.begin(), orthogonal_moves.end(), square_moves.begin());
        }
        
        pseudolegal_moves.insert(square_moves.begin(), square_moves.end());
      }
    }
  }

  return pseudolegal_moves;
}

set<Move> MoveGen::GetPawnMoves(Square square) {
  set<Move> moves;
  Color color = position.GetActiveColor();
  
  return moves;
}

set<Move> MoveGen::GetKnightMoves(Square square) {
  set<Move> moves;
  
  return moves;
}

set<Move> MoveGen::GetDiagonalMoves(Square square, bool limit_to_one) {
  set<Move> moves;
  
  return moves;
}

set<Move> MoveGen::GetOrthogonalMoves(Square square, bool limit_to_one) {
  set<Move> moves;
  
  return moves;
}

}

