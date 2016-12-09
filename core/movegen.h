#ifndef MOVEGEN_H__
#define MOVEGEN_H__

#include "move.h"
#include "position.h"

#include <vector>

namespace core {

class MoveGen {
public:
  static std::vector<Move> AllPseudolegalMoves(Position position);
  static std::vector<Move> AllLegalMoves(Position position);
  
  static bool IsPseudolegalMoveLegal(Position position, Move move);
  static bool IsInCheck(Position position, std::vector<Square> king_squares, Color color);

private:
  static Position position;
  
  MoveGen();
  
  static std::vector<Move> GetPawnMoves(Square square);
  static std::vector<Move> GetKnightMoves(Square square);
  static std::vector<Move> GetDiagonalMoves(Square square, bool limit_to_one);
  static std::vector<Move> GetOrthogonalMoves(Square square, bool limit_to_one);
  
  static bool IsValidDestSquare(Square square, Color color);
  static bool IsPawnCaptureSquare(Square square, Color color);
  static bool IsCaptureSquare(Square square, Color color);
  
  static std::vector<Square> GetKnightSquares(Square square);
  static Square LineEndingSquare(Position after_move, Square start_square,
                                 int rank_increment, int file_increment);
  static bool CanAttack(SquareContents source_contents, Square source_square, Square dest_square);
  
  static void AddLineMoves(Square square, int rank_increment, int column_increment,
                           bool limit_to_one, std::vector<Move> *moves);
  static std::vector<Move> GetPromotionMoves(Square start_square, Square end_square);
};

}

#endif
