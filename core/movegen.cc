#include "movegen.h"

#include <algorithm>
#include <string>
#include <vector>
using std::vector;

namespace core {

MoveGen::MoveGen() {}

vector<Move> MoveGen::AllLegalMoves(Position *position) {
  vector<Move> legal_moves = AllPseudolegalMoves(*position);

  for (vector<Move>::iterator it = legal_moves.begin(); it != legal_moves.end(); ) {
    Move move = *it;
    if (!IsPseudolegalMoveLegal(position, move)) {
      it = legal_moves.erase(it);
    } else {
      ++it;
    }
  }

  return legal_moves;
}

vector<Move> MoveGen::AllPseudolegalMoves(const Position& position) {
  vector<Move> pseudolegal_moves;
  Color color = position.GetActiveColor();

  int start_rank = color == WHITE ? 7 : 0;

  for (int rank = start_rank; rank >= 0 && rank <= 7; rank += (color == WHITE ? -1 : 1)) {
    for (int file = 0; file < 8; file++) {
      Square square = Square(rank, file);
      SquareContents contents = position.ContentsAt(square);
      if (ColorOfContents(contents) == color) {

        PieceType piece_type = GetPieceType(contents);
        if (piece_type == PAWN) {
          GetPawnMoves(position, square, &pseudolegal_moves);
        } else if (piece_type == KNIGHT) {
          GetKnightMoves(position, square, &pseudolegal_moves);
        } else if (piece_type == BISHOP) {
          GetDiagonalMoves(position, square, false, &pseudolegal_moves);
        } else if (piece_type == ROOK) {
          GetOrthogonalMoves(position, square, false, &pseudolegal_moves);
        } else if (piece_type == QUEEN) {
          GetDiagonalMoves(position, square, false, &pseudolegal_moves);
          GetOrthogonalMoves(position, square, false, &pseudolegal_moves);
        } else if (piece_type == KING) {
          GetDiagonalMoves(position, square, true, &pseudolegal_moves);
          GetOrthogonalMoves(position, square, true, &pseudolegal_moves);

          if (contents == KING_W && square == e1 &&
            position.ContentsAt(f1) == EMPTY && position.ContentsAt(g1) == EMPTY &&
            position.ContentsAt(h1) == ROOK_W && position.GetCastle().white_kingside) {

            pseudolegal_moves.push_back(Move(e1, g1, position));
          }
          if (contents == KING_W && square == e1 &&
            position.ContentsAt(d1) == EMPTY && position.ContentsAt(c1) == EMPTY &&
            position.ContentsAt(b1) == EMPTY && position.ContentsAt(a1) == ROOK_W &&
            position.GetCastle().white_queenside) {

            pseudolegal_moves.push_back(Move(e1, c1, position));
          }
          if (contents == KING_B && square == e8 &&
            position.ContentsAt(f8) == EMPTY && position.ContentsAt(g8) == EMPTY &&
            position.ContentsAt(h8) == ROOK_B && position.GetCastle().black_kingside) {

            pseudolegal_moves.push_back(Move(e8, g8, position));
          }
          if (contents == KING_B && square == e8 &&
            position.ContentsAt(d8) == EMPTY && position.ContentsAt(c8) == EMPTY &&
            position.ContentsAt(b8) == EMPTY && position.ContentsAt(a8) == ROOK_B &&
            position.GetCastle().black_queenside) {

            pseudolegal_moves.push_back(Move(e8, c8, position));
          }

        }
      }
    }
  }

  vector<Move> ordered_pseudolegal_moves;
  for (unsigned int i = 0; i < pseudolegal_moves.size(); i++) {
    Move move = pseudolegal_moves[i];
    if (move.is_capture) {
      ordered_pseudolegal_moves.push_back(move);
    }
  }
  for (unsigned int i = 0; i < pseudolegal_moves.size(); i++) {
    Move move = pseudolegal_moves[i];
    if (!move.is_capture && IsCentralSquare(move.end_square)) {
      ordered_pseudolegal_moves.push_back(move);
    }
  }
  for (unsigned int i = 0; i < pseudolegal_moves.size(); i++) {
    Move move = pseudolegal_moves[i];
    if (!move.is_capture && !IsCentralSquare(move.end_square)) {
      ordered_pseudolegal_moves.push_back(move);
    }
  }

  return ordered_pseudolegal_moves;
}

bool MoveGen::IsPseudolegalMoveLegal(Position *position, const Move& move) {
  Color color = position->GetActiveColor();
  position->PerformMove(move);

  vector<Square> king_squares;
  king_squares.push_back(position->FindKing(color));

  if (move.is_castle) {
    if (move.start_square == e1 && move.end_square == g1) {
      king_squares.push_back(e1);
      king_squares.push_back(f1);
    } else if (move.start_square == e1 && move.end_square == c1) {
      king_squares.push_back(e1);
      king_squares.push_back(d1);
    } else if (move.start_square == e8 && move.end_square == g8) {
      king_squares.push_back(e8);
      king_squares.push_back(f8);
    } else if (move.start_square == e8 && move.end_square == c8) {
      king_squares.push_back(e8);
      king_squares.push_back(d8);
    }
  }

  bool is_in_check = IsInCheck(*position, king_squares, color);
  position->UndoLastMove();

  return !is_in_check;
}

bool MoveGen::IsInCheck(const Position& position, const vector<Square>& king_squares, Color color) {
  for (Square king_square : king_squares) {

    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, 1, 0), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, 1, 1), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, 0, 1), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, -1, 1), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, -1, 0), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, -1, -1), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, 0, -1), color)) return true;
    if (CanLineEndingSquareAttack(position, king_square, LineEndingSquare(position, king_square, 1, -1), color)) return true;

    SquareContents enemy_knight = MakePiece(KNIGHT, position.GetActiveColor());
    vector<Square> knight_attack_squares = GetKnightSquares(king_square);
    for (Square knight_square : knight_attack_squares) {
      if (knight_square.is_real_square && position.ContentsAt(knight_square) == enemy_knight) {
        return true;
      }
    }

  }
  return false;
}

bool MoveGen::CanLineEndingSquareAttack(const Position& position, const Square& target_square, 
                                        const Square& attack_square, Color color) {
  if (!attack_square.is_real_square) {
    return false;
  }
  SquareContents attack_contents = position.ContentsAt(attack_square);
  if (ColorOfContents(attack_contents) == color) {
    return false;
  }
  if (CanAttack(attack_contents, attack_square, target_square)) {
    return true;
  }
  return false;
}

void MoveGen::GetPawnMoves(const Position& position, const Square& square, vector<Move> *moves) {
  Color color = position.GetActiveColor();

  bool is_second_rank = square.rank == 1;
  bool is_seventh_rank = square.rank == 6;

  bool is_promotion = (color == WHITE && is_seventh_rank) ||
    (color == BLACK && is_second_rank);

  Square one_ahead, two_ahead, capture_left, capture_right;
  bool one_ahead_valid = true, two_ahead_valid = true,
    capture_left_valid = true, capture_right_valid = true;
  if (color == WHITE) {
    one_ahead = Square(square.rank + 1, square.file);
    two_ahead = Square(square.rank + 2, square.file);
    capture_left = Square(square.rank + 1, square.file - 1);
    capture_right = Square(square.rank + 1, square.file + 1);
  } else {
    one_ahead = Square(square.rank - 1, square.file);
    two_ahead = Square(square.rank - 2, square.file);
    capture_left = Square(square.rank - 1, square.file - 1);
    capture_right = Square(square.rank - 1, square.file + 1);
  }

  if (!IsValidDestSquare(position, one_ahead, color)) {
    one_ahead_valid = false;
    two_ahead_valid = false;
  }
  if (two_ahead_valid && !IsValidDestSquare(position, two_ahead, color)) {
    two_ahead_valid = false;
  }
  if (!IsValidDestSquare(position, capture_left, color)) {
    capture_left_valid = false;
  }
  if (!IsValidDestSquare(position, capture_right, color)) {
    capture_right_valid = false;
  }

  if (two_ahead_valid && color == WHITE && !is_second_rank) {
    two_ahead_valid = false;
  }
  if (two_ahead_valid && color == BLACK && !is_seventh_rank) {
    two_ahead_valid = false;
  }

  if (IsPawnCaptureSquare(position, one_ahead, color)) {
    one_ahead_valid = false;
    two_ahead_valid = false;
  }
  if (two_ahead_valid && IsPawnCaptureSquare(position, two_ahead, color)) {
    two_ahead_valid = false;
  }

  if (capture_left_valid && !IsPawnCaptureSquare(position, capture_left, color)) {
    capture_left_valid = false;
  }
  if (capture_right_valid && !IsPawnCaptureSquare(position, capture_right, color)) {
    capture_right_valid = false;
  }

  if (one_ahead_valid) {
    if (is_promotion) {
      GetPromotionMoves(position, square, one_ahead, moves);
    } else {
      moves->push_back(Move(square, one_ahead, position));
    }
  }
  if (two_ahead_valid) {
    moves->push_back(Move(square, two_ahead, position));
  }
  if (capture_left_valid) {
    if (is_promotion) {
      GetPromotionMoves(position, square, capture_left, moves);
    } else {
      moves->push_back(Move(square, capture_left, position));
    }
  }
  if (capture_right_valid) {
    if (is_promotion) {
      GetPromotionMoves(position, square, capture_right, moves);
    } else {
      moves->push_back(Move(square, capture_right, position));
    }
  }
}

void MoveGen::GetPromotionMoves(const Position& position, const Square& start_square,
    const Square& end_square, vector<Move> *moves) {

  moves->push_back(Move(start_square, end_square, position, QUEEN));
  moves->push_back(Move(start_square, end_square, position, ROOK));
  moves->push_back(Move(start_square, end_square, position, KNIGHT));
  moves->push_back(Move(start_square, end_square, position, BISHOP));
}

void MoveGen::GetKnightMoves(const Position& position, const Square& square, vector<Move> *moves) {
  vector<Square> dest_squares = GetKnightSquares(square);

  Color color = position.GetActiveColor();

  for (Square dest : dest_squares) {
    if (IsValidDestSquare(position, dest, color)) {
      moves->push_back(Move(square, dest, position));
    }
  }
}

void MoveGen::GetDiagonalMoves(const Position& position, const Square& square,
                               bool limit_to_one, vector<Move> *moves) {

  AddLineMoves(position, square, 1, 1, limit_to_one, moves);
  AddLineMoves(position, square, 1, -1, limit_to_one, moves);
  AddLineMoves(position, square, -1, -1, limit_to_one, moves);
  AddLineMoves(position, square, -1, 1, limit_to_one, moves);
}

void MoveGen::GetOrthogonalMoves(const Position& position, const Square& square,
                                         bool limit_to_one, vector<Move> *moves) {

  AddLineMoves(position, square, 1, 0, limit_to_one, moves);
  AddLineMoves(position, square, 0, -1, limit_to_one, moves);
  AddLineMoves(position, square, -1, 0, limit_to_one, moves);
  AddLineMoves(position, square, 0, 1, limit_to_one, moves);
}

void MoveGen::AddLineMoves(const Position& position, const Square& square, int rank_increment,
                           int file_increment, bool limit_to_one, vector<Move> *moves) {
  Color color = position.GetActiveColor();
  Square dest_square = square;
  bool search_onward = true;
  while (search_onward) {
    dest_square = Square(dest_square.rank + rank_increment,
                         dest_square.file + file_increment);
    if (IsValidDestSquare(position, dest_square, color)) {
      moves->push_back(Move(square, dest_square, position));
      if (IsCaptureSquare(position, dest_square, color)) {
        search_onward = false;
      }
    } else {
      search_onward = false;
    }

    if (limit_to_one) {
      search_onward = false;
    }
  }
}

Square MoveGen::LineEndingSquare(const Position& position, const Square& start_square,
                                 int rank_increment, int file_increment) {
  Square next_square = Square(start_square.rank + rank_increment,
    start_square.file + file_increment);
  while (next_square.is_real_square && position.ContentsAt(next_square) == EMPTY) {
    next_square = Square(next_square.rank + rank_increment, next_square.file + file_increment);
  }
  return next_square;
}

// Assumes all in-between squares are empty.
bool MoveGen::CanAttack(SquareContents attack_contents, const Square& attack_square, const Square& target_square) {
  if (attack_contents == PAWN_W) {
    Square capture_left = Square(attack_square.rank + 1, attack_square.file - 1);
    Square capture_right = Square(attack_square.rank + 1, attack_square.file + 1);
    return (target_square == capture_left || target_square == capture_right);
  }
  if (attack_contents == PAWN_B) {
    Square capture_left = Square(attack_square.rank - 1, attack_square.file - 1);
    Square capture_right = Square(attack_square.rank - 1, attack_square.file + 1);
    return (target_square == capture_left || target_square == capture_right);
  }

  PieceType attacking_piece = GetPieceType(attack_contents);
  switch (attacking_piece) {
    case KING: return SquareDistance(attack_square, target_square) <= 1;
    case BISHOP: return SquaresAreDiagonal(attack_square, target_square);
    case ROOK: return SquaresAreOrthogonal(attack_square, target_square);
    case QUEEN: return SquaresAreDiagonal(attack_square, target_square) || SquaresAreOrthogonal(attack_square, target_square);
    default: return false;
  }
  return false;
}

bool MoveGen::IsValidDestSquare(const Position& position, const Square& square, Color color) {
  return square.is_real_square &&
    ColorOfContents(position.ContentsAt(square)) != color;
}

bool MoveGen::IsCaptureSquare(const Position& position, const Square& square, Color color) {
  if (!square.is_real_square) {
    return false;
  } else if (OppositeColors(color, ColorOfContents(position.ContentsAt(square)))) {
    return true;
  } else {
    return false;
  }
}

bool MoveGen::IsPawnCaptureSquare(const Position& position, const Square& square, Color color) {
  if (!square.is_real_square) {
    return false;
  } else if (OppositeColors(color, ColorOfContents(position.ContentsAt(square)))) {
    return true;
  } else if (square == position.GetEnPassant()) {
    return true;
  } else {
    return false;
  }
}

vector<Square> MoveGen::GetKnightSquares(const Square& square) {
  vector<Square> dest_squares = {
    Square(square.rank + 2, square.file + 1),
    Square(square.rank + 1, square.file + 2),
    Square(square.rank - 1, square.file + 2),
    Square(square.rank - 2, square.file + 1),
    Square(square.rank - 2, square.file - 1),
    Square(square.rank - 1, square.file - 2),
    Square(square.rank + 1, square.file - 2),
    Square(square.rank + 2, square.file - 1)
  };

  return dest_squares;
}

}
