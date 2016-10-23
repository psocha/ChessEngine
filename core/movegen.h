#ifndef MOVEGEN_H__
#define MOVEGEN_H__

#include "move.h"
#include "position.h"

#include <set>
#include <vector>

namespace core {

class MoveGen {
public:
  static std::set<Move> AllLegalMoves(Position position);

private:
  static Position position;
  
  MoveGen();

  static std::set<Move> AllPseudolegalMoves();
  static void PruneCheckMoves(std::set<Move> *legal_moves);
  static bool IsInCheck(Position after_move, std::vector<Square> king_squares, Color color);
  
  static std::set<Move> GetPawnMoves(Square square);
  static std::set<Move> GetKnightMoves(Square square);
  static std::set<Move> GetDiagonalMoves(Square square, bool limit_to_one);
  static std::set<Move> GetOrthogonalMoves(Square square, bool limit_to_one);
  
  static bool IsValidDestSquare(Square square, Color color);
  static bool IsPawnCaptureSquare(Square square, Color color);
  static bool IsCaptureSquare(Square square, Color color);
  
  static std::vector<Square> GetKnightSquares(Square square);
  static Square LineEndingSquare(Position after_move, Square start_square,
                                 int rank_increment, int file_increment);
  static bool CanAttack(SquareContents source_contents, Square source_square, Square dest_square);
  
  static void AddLineMoves(Square square, int rank_increment, int column_increment,
                           bool limit_to_one, std::set<Move> *moves);
  static std::set<Move> GetPromotionMoves(Square start_square, Square end_square);
};

}

#endif
