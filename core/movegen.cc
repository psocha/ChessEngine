#include "movegen.h"

#include <algorithm>
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

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Square square = Square(rank, file);
      SquareContents contents = position.ContentsAt(square);
      if (ColorOfContents(contents) == color) {
        vector<Move> square_moves;

        PieceType piece_type = GetPieceType(contents);
        if (piece_type == PAWN) {
          square_moves = GetPawnMoves(position, square);
        } else if (piece_type == KNIGHT) {
          square_moves = GetKnightMoves(position, square);
        } else if (piece_type == BISHOP) {
          square_moves = GetDiagonalMoves(position, square, false);
        } else if (piece_type == ROOK) {
          square_moves = GetOrthogonalMoves(position, square, false);
        } else if (piece_type == QUEEN) {
          vector<Move> diagonal_moves = GetDiagonalMoves(position, square, false);
          vector<Move> orthogonal_moves = GetOrthogonalMoves(position, square, false);
          for (Move move : diagonal_moves) {
            square_moves.push_back(move);
          }
          for (Move move : orthogonal_moves) {
            square_moves.push_back(move);
          }
        } else if (piece_type == KING) {
          vector<Move> diagonal_moves = GetDiagonalMoves(position, square, true);
          vector<Move> orthogonal_moves = GetOrthogonalMoves(position, square, true);
          for (Move move : diagonal_moves) {
            square_moves.push_back(move);
          }
          for (Move move : orthogonal_moves) {
            square_moves.push_back(move);
          }

          if (contents == KING_W && square == Square("e1") &&
            position.ContentsAt(Square("f1")) == EMPTY && position.ContentsAt(Square("g1")) == EMPTY &&
            position.ContentsAt(Square("h1")) == ROOK_W && position.GetCastle().white_kingside) {

            square_moves.push_back(Move("e1g1", position));
          }
          if (contents == KING_W && square == Square("e1") &&
            position.ContentsAt(Square("d1")) == EMPTY && position.ContentsAt(Square("c1")) == EMPTY &&
            position.ContentsAt(Square("b1")) == EMPTY && position.ContentsAt(Square("a1")) == ROOK_W &&
            position.GetCastle().white_queenside) {

            square_moves.push_back(Move("e1c1", position));
          }
          if (contents == KING_B && square == Square("e8") &&
            position.ContentsAt(Square("f8")) == EMPTY && position.ContentsAt(Square("g8")) == EMPTY &&
            position.ContentsAt(Square("h8")) == ROOK_B && position.GetCastle().black_kingside) {

            square_moves.push_back(Move("e8g8", position));
          }
          if (contents == KING_B && square == Square("e8") &&
            position.ContentsAt(Square("d8")) == EMPTY && position.ContentsAt(Square("c8")) == EMPTY &&
            position.ContentsAt(Square("b8")) == EMPTY && position.ContentsAt(Square("a8")) == ROOK_B &&
            position.GetCastle().black_queenside) {

            square_moves.push_back(Move("e8c8", position));
          }

        }
        for (Move square_move : square_moves) {
          pseudolegal_moves.push_back(square_move);
        }
      }
    }
  }

  vector<Move> ordered_pseudolegal_moves;
  for (unsigned int i = 0; i < pseudolegal_moves.size(); i++) {
    Move move = pseudolegal_moves.at(i);
    if (move.is_capture) {
      ordered_pseudolegal_moves.push_back(move);
    }
  }
  for (unsigned int i = 0; i < pseudolegal_moves.size(); i++) {
    Move move = pseudolegal_moves.at(i);
    if (!move.is_capture) {
      ordered_pseudolegal_moves.push_back(move);
    }
  }

  return ordered_pseudolegal_moves;
}

bool MoveGen::IsPseudolegalMoveLegal(Position *position, Move move) {
  Color color = position->GetActiveColor();
  position->PerformMove(move.ToString());

  vector<Square> king_squares;
  king_squares.push_back(position->FindKing(color));

  if (move.is_castle && move.ToString() == "e1g1") {
    king_squares.push_back(Square("e1"));
    king_squares.push_back(Square("f1"));
  }
  if (move.is_castle && move.ToString() == "e1c1") {
    king_squares.push_back(Square("e1"));
    king_squares.push_back(Square("d1"));
  }
  if (move.is_castle && move.ToString() == "e8g8") {
    king_squares.push_back(Square("e8"));
    king_squares.push_back(Square("f8"));
  }
  if (move.is_castle && move.ToString() == "e8c8") {
    king_squares.push_back(Square("e8"));
    king_squares.push_back(Square("d8"));
  }

  bool is_in_check = IsInCheck(*position, king_squares, color);
  position->UndoLastMove();

  return !is_in_check;
}

bool MoveGen::IsInCheck(const Position& position, vector<Square> king_squares, Color color) {
  for (Square king_square : king_squares) {

    SquareContents enemy_knight = MakePiece(KNIGHT, position.GetActiveColor());
    vector<Square> knight_attack_squares = GetKnightSquares(king_square);
    for (Square knight_square : knight_attack_squares) {
      if (knight_square.is_real_square && position.ContentsAt(knight_square) == enemy_knight) {
        return true;
      }
    }

    vector<Square> line_endings;
    line_endings.push_back(LineEndingSquare(position, king_square, 1, 0));
    line_endings.push_back(LineEndingSquare(position, king_square, 1, 1));
    line_endings.push_back(LineEndingSquare(position, king_square, 0, 1));
    line_endings.push_back(LineEndingSquare(position, king_square, -1, 1));
    line_endings.push_back(LineEndingSquare(position, king_square, -1, 0));
    line_endings.push_back(LineEndingSquare(position, king_square, -1, -1));
    line_endings.push_back(LineEndingSquare(position, king_square, 0, -1));
    line_endings.push_back(LineEndingSquare(position, king_square, 1, -1));

    for (Square line_ending : line_endings) {
      if (!line_ending.is_real_square) {
        continue;
      }
      SquareContents line_ending_contents = position.ContentsAt(line_ending);
      if (ColorOfContents(line_ending_contents) == color) {
        continue;
      }

      if (CanAttack(line_ending_contents, line_ending, king_square)) {
        return true;
      }
    }

  }
  return false;
}

vector<Move> MoveGen::GetPawnMoves(const Position& position, Square square) {
  vector<Move> moves;
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
  if (!IsValidDestSquare(position, two_ahead, color)) {
    two_ahead_valid = false;
  }
  if (!IsValidDestSquare(position, capture_left, color)) {
    capture_left_valid = false;
  }
  if (!IsValidDestSquare(position, capture_right, color)) {
    capture_right_valid = false;
  }

  if (color == WHITE && !is_second_rank) {
    two_ahead_valid = false;
  }
  if (color == BLACK && !is_seventh_rank) {
    two_ahead_valid = false;
  }

  if (IsPawnCaptureSquare(position, one_ahead, color)) {
    one_ahead_valid = false;
    two_ahead_valid = false;
  }
  if (IsPawnCaptureSquare(position, two_ahead, color)) {
    two_ahead_valid = false;
  }

  if (!IsPawnCaptureSquare(position, capture_left, color)) {
    capture_left_valid = false;
  }
  if (!IsPawnCaptureSquare(position, capture_right, color)) {
    capture_right_valid = false;
  }

  if (one_ahead_valid) {
    if (is_promotion) {
      vector<Move> promotions = GetPromotionMoves(position, square, one_ahead);
      for (Move promotion : promotions) {
        moves.push_back(promotion);
      }
    } else {
      moves.push_back(Move(square, one_ahead, position));
    }
  }
  if (two_ahead_valid) {
    moves.push_back(Move(square, two_ahead, position));
  }
  if (capture_left_valid) {
    if (is_promotion) {
      vector<Move> promotions = GetPromotionMoves(position, square, capture_left);
      for (Move promotion : promotions) {
        moves.push_back(promotion);
      }
    } else {
      moves.push_back(Move(square, capture_left, position));
    }
  }
  if (capture_right_valid) {
    if (is_promotion) {
      vector<Move> promotions = GetPromotionMoves(position, square, capture_right);
      for (Move promotion : promotions) {
        moves.push_back(promotion);
      }
    } else {
      moves.push_back(Move(square, capture_right, position));
    }
  }

  return moves;
}

vector<Move> MoveGen::GetKnightMoves(const Position& position, Square square) {
  vector<Square> dest_squares = GetKnightSquares(square);

  Color color = position.GetActiveColor();
  vector<Move> moves;

  for (Square dest : dest_squares) {
    if (IsValidDestSquare(position, dest, color)) {
      moves.push_back(Move(square, dest, position));
    }
  }

  return moves;
}

vector<Move> MoveGen::GetDiagonalMoves(const Position& position, Square square,
                                       bool limit_to_one) {
  vector<Move> moves;

  AddLineMoves(position, square, 1, 1, limit_to_one, &moves);
  AddLineMoves(position, square, 1, -1, limit_to_one, &moves);
  AddLineMoves(position, square, -1, -1, limit_to_one, &moves);
  AddLineMoves(position, square, -1, 1, limit_to_one, &moves);

  return moves;
}

vector<Move> MoveGen::GetOrthogonalMoves(const Position& position, Square square,
                                         bool limit_to_one) {
  vector<Move> moves;

  AddLineMoves(position, square, 1, 0, limit_to_one, &moves);
  AddLineMoves(position, square, 0, -1, limit_to_one, &moves);
  AddLineMoves(position, square, -1, 0, limit_to_one, &moves);
  AddLineMoves(position, square, 0, 1, limit_to_one, &moves);

  return moves;
}

void MoveGen::AddLineMoves(const Position& position, Square square, int rank_increment,
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

Square MoveGen::LineEndingSquare(const Position& position, Square start_square,
                                 int rank_increment, int file_increment) {
  Square next_square = Square(start_square.rank + rank_increment,
    start_square.file + file_increment);
  while (next_square.is_real_square && position.ContentsAt(next_square) == EMPTY) {
    next_square = Square(next_square.rank + rank_increment, next_square.file + file_increment);
  }
  return next_square;
}

// Assumes all in-between squares are empty.
bool MoveGen::CanAttack(SquareContents source_contents, Square source_square, Square dest_square) {
  if (source_contents == PAWN_W) {
    Square capture_left = Square(source_square.rank + 1, source_square.file - 1);
    Square capture_right = Square(source_square.rank + 1, source_square.file + 1);
    return (dest_square == capture_left || dest_square == capture_right);
  }
  if (source_contents == PAWN_B) {
    Square capture_left = Square(source_square.rank - 1, source_square.file - 1);
    Square capture_right = Square(source_square.rank - 1, source_square.file + 1);
    return (dest_square == capture_left || dest_square == capture_right);
  }

  PieceType attacking_piece = GetPieceType(source_contents);
  if (attacking_piece == KING) {
    return SquareDistance(source_square, dest_square) <= 1;
  } else if (attacking_piece == BISHOP) {
    return SquaresAreDiagonal(source_square, dest_square);
  } else if (attacking_piece == ROOK) {
    return SquaresAreOrthogonal(source_square, dest_square);
  } else if (attacking_piece == QUEEN) {
    return SquaresAreDiagonal(source_square, dest_square) ||
      SquaresAreOrthogonal(source_square, dest_square);
  }
  return false;
}

bool MoveGen::IsValidDestSquare(const Position& position, Square square, Color color) {
  return square.is_real_square &&
    ColorOfContents(position.ContentsAt(square)) != color;
}

bool MoveGen::IsCaptureSquare(const Position& position, Square square, Color color) {
    if (!square.is_real_square) {
    return false;
  } else if (OppositeColors(color, ColorOfContents(position.ContentsAt(square)))) {
    return true;
  } else {
    return false;
  }
}

bool MoveGen::IsPawnCaptureSquare(const Position& position, Square square, Color color) {
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

vector<Square> MoveGen::GetKnightSquares(Square square) {
  vector<Square> dest_squares;
  dest_squares.push_back(Square(square.rank + 2, square.file + 1));
  dest_squares.push_back(Square(square.rank + 1, square.file + 2));
  dest_squares.push_back(Square(square.rank - 1, square.file + 2));
  dest_squares.push_back(Square(square.rank - 2, square.file + 1));
  dest_squares.push_back(Square(square.rank - 2, square.file - 1));
  dest_squares.push_back(Square(square.rank - 1, square.file - 2));
  dest_squares.push_back(Square(square.rank + 1, square.file - 2));
  dest_squares.push_back(Square(square.rank + 2, square.file - 1));

  return dest_squares;
}

vector<Move> MoveGen::GetPromotionMoves(const Position& position,
    Square start_square, Square end_square) {
  vector<Move> promotions;

  promotions.push_back(Move(start_square, end_square, position, KNIGHT));
  promotions.push_back(Move(start_square, end_square, position, BISHOP));
  promotions.push_back(Move(start_square, end_square, position, ROOK));
  promotions.push_back(Move(start_square, end_square, position, QUEEN));

  return promotions;
}

}
