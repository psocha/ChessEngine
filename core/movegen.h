#ifndef MOVEGEN_H__
#define MOVEGEN_H__

#include "move.h"
#include "position.h"

#include <vector>

namespace core {

class MoveGen {
public:
  static std::vector<Move> AllPseudolegalMoves(const Position& position);
  static std::vector<Move> AllLegalMoves(Position *position);

  static bool IsPseudolegalMoveLegal(Position *position, const Move& move);
  static bool IsInCheck(const Position& position, const std::vector<Square>& king_squares, Color color);

private:
  MoveGen();

  static std::vector<Move> GetPawnMoves(const Position& position, const Square& square);
  static std::vector<Move> GetKnightMoves(const Position& position, const Square& square);
  static std::vector<Move> GetDiagonalMoves(const Position& position, const Square& square, bool limit_to_one);
  static std::vector<Move> GetOrthogonalMoves(const Position& position, const Square& square, bool limit_to_one);

  static bool IsValidDestSquare(const Position& position, const Square& square, Color color);
  static bool IsPawnCaptureSquare(const Position& position, const Square& square, Color color);
  static bool IsCaptureSquare(const Position& position, const Square& square, Color color);

  static std::vector<Square> GetKnightSquares(const Square& square);
  static Square LineEndingSquare(const Position& position, const Square& start_square,
                                 int rank_increment, int file_increment);
  static bool CanAttack(SquareContents attack_contents, const Square& attack_square, const Square& target_square);
  static bool CanLineEndingSquareAttack(const Position& position, const Square& target_square, const Square& attack_square, Color color);

  static void AddLineMoves(const Position& position, const Square& square, int rank_increment, int column_increment,
                           bool limit_to_one, std::vector<Move> *moves);
  static std::vector<Move> GetPromotionMoves(const Position& position, const Square& start_square, const Square& end_square);
};

}

#endif
