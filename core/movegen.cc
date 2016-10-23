#include "movegen.h"

#include <algorithm>
#include <vector>
using std::set;
using std::vector;

namespace core {

MoveGen::MoveGen() {}

MoveGen::~MoveGen() {}

set<Move> MoveGen::AllLegalMoves(Position position) {
  this->position = position;
  
  set<Move> legal_moves = AllPseudolegalMoves();
  PruneCheckMoves(&legal_moves);

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
          for (Move move : diagonal_moves) {
            square_moves.insert(move);
          }
          for (Move move : orthogonal_moves) {
            square_moves.insert(move);
          }
        } else if (piece_type == KING) {
          set<Move> diagonal_moves = GetDiagonalMoves(square, true);
          set<Move> orthogonal_moves = GetOrthogonalMoves(square, true);
          for (Move move : diagonal_moves) {
            square_moves.insert(move);
          }
          for (Move move : orthogonal_moves) {
            square_moves.insert(move);
          }
          
          if (contents == KING_W && square == Square("e1") &&
            position.ContentsAt(Square("f1")) == EMPTY && position.ContentsAt(Square("g1")) == EMPTY &&
            position.ContentsAt(Square("h1")) == ROOK_W && position.GetCastle().white_kingside) {
            
            square_moves.insert(Move("e1g1", position));
          }
          if (contents == KING_W && square == Square("e1") &&
            position.ContentsAt(Square("d1")) == EMPTY && position.ContentsAt(Square("c1")) == EMPTY &&
            position.ContentsAt(Square("b1")) == EMPTY && position.ContentsAt(Square("a1")) == ROOK_W &&
            position.GetCastle().white_queenside) {
            
            square_moves.insert(Move("e1c1", position));
          }
          if (contents == KING_B && square == Square("e8") &&
            position.ContentsAt(Square("f8")) == EMPTY && position.ContentsAt(Square("g8")) == EMPTY &&
            position.ContentsAt(Square("h8")) == ROOK_B && position.GetCastle().black_kingside) {
            
            square_moves.insert(Move("e8g8", position));
          }
          if (contents == KING_B && square == Square("e8") &&
            position.ContentsAt(Square("d8")) == EMPTY && position.ContentsAt(Square("c8")) == EMPTY &&
            position.ContentsAt(Square("b8")) == EMPTY && position.ContentsAt(Square("a8")) == ROOK_B &&
            position.GetCastle().black_queenside) {
            
            square_moves.insert(Move("e8c8", position));
          }
          
        }
        pseudolegal_moves.insert(square_moves.begin(), square_moves.end());
      }
    }
  }
  return pseudolegal_moves;
}

void MoveGen::PruneCheckMoves(set<Move> *legal_moves) {
  Color color = position.GetActiveColor();
  Position after_move;
  
  for (set<Move>::iterator it = legal_moves->begin(); it != legal_moves->end(); ) {
    Move move = *it;
    after_move = position;
    after_move.PerformMove(move.ToString());
    
    vector<Square> king_squares;
    king_squares.push_back(after_move.FindKing(color));
    
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
    
    if (IsInCheck(after_move, king_squares, color)) {
      it = legal_moves->erase(it);
    } else {
      ++it;
    }
  }
}

bool MoveGen::IsInCheck(Position after_move,vector<Square> king_squares, Color color) {
  for (Square king_square : king_squares) {
    
    SquareContents enemy_knight = MakePiece(KNIGHT, after_move.GetActiveColor());
    vector<Square> knight_attack_squares = GetKnightSquares(king_square);
    for (Square knight_square : knight_attack_squares) {
      if (knight_square.is_real_square && after_move.ContentsAt(knight_square) == enemy_knight) {
        return true;
      }
    }
    
    vector<Square> line_endings;
    line_endings.push_back(LineEndingSquare(after_move, king_square, 1, 0));
    line_endings.push_back(LineEndingSquare(after_move, king_square, 1, 1));
    line_endings.push_back(LineEndingSquare(after_move, king_square, 0, 1));
    line_endings.push_back(LineEndingSquare(after_move, king_square, -1, 1));
    line_endings.push_back(LineEndingSquare(after_move, king_square, -1, 0));
    line_endings.push_back(LineEndingSquare(after_move, king_square, -1, -1));
    line_endings.push_back(LineEndingSquare(after_move, king_square, 0, -1));
    line_endings.push_back(LineEndingSquare(after_move, king_square, 1, -1));
    
    for (Square line_ending : line_endings) {
      if (!line_ending.is_real_square) {
        continue;
      }
      SquareContents line_ending_contents = after_move.ContentsAt(line_ending);
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

set<Move> MoveGen::GetPawnMoves(Square square) {
  set<Move> moves;
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

  if (!IsValidDestSquare(one_ahead, color)) {
    one_ahead_valid = false;
    two_ahead_valid = false;
  }
  if (!IsValidDestSquare(two_ahead, color)) {
    two_ahead_valid = false;
  }
  if (!IsValidDestSquare(capture_left, color)) {
    capture_left_valid = false;
  }
  if (!IsValidDestSquare(capture_right, color)) {
    capture_right_valid = false;
  }
  
  if (color == WHITE && !is_second_rank) {
    two_ahead_valid = false;
  }
  if (color == BLACK && !is_seventh_rank) {
    two_ahead_valid = false;
  }
  
  if (IsPawnCaptureSquare(one_ahead, color)) {
    one_ahead_valid = false;
    two_ahead_valid = false;
  }
  if (IsPawnCaptureSquare(two_ahead, color)) {
    two_ahead_valid = false;
  }
  
  if (!IsPawnCaptureSquare(capture_left, color)) {
    capture_left_valid = false;
  }
  if (!IsPawnCaptureSquare(capture_right, color)) {
    capture_right_valid = false;
  }
 
  if (one_ahead_valid) {
    if (is_promotion) {
      set<Move> promotions = GetPromotionMoves(square, one_ahead);
      for (Move promotion : promotions) {
        moves.insert(promotion);
      }
    } else {
      moves.insert(Move(square, one_ahead, position));
    }
  }
  if (two_ahead_valid) {
    moves.insert(Move(square, two_ahead, position));
  }
  if (capture_left_valid) {
    if (is_promotion) {
      set<Move> promotions = GetPromotionMoves(square, capture_left);
      for (Move promotion : promotions) {
        moves.insert(promotion);
      }
    } else {
      moves.insert(Move(square, capture_left, position));
    }
  }
  if (capture_right_valid) {
    if (is_promotion) {
      set<Move> promotions = GetPromotionMoves(square, capture_right);
      for (Move promotion : promotions) {
        moves.insert(promotion);
      }
    } else {
      moves.insert(Move(square, capture_right, position));
    }
  }

  return moves;
}

set<Move> MoveGen::GetKnightMoves(Square square) {
  vector<Square> dest_squares = GetKnightSquares(square);
  
  Color color = position.GetActiveColor();
  set<Move> moves;
  
  for (Square dest : dest_squares) {
    if (IsValidDestSquare(dest, color)) {
      moves.insert(Move(square, dest, position));
    }
  }
  
  return moves;
}

set<Move> MoveGen::GetDiagonalMoves(Square square, bool limit_to_one) {
  set<Move> moves;
  
  AddLineMoves(square, 1, 1, limit_to_one, &moves);
  AddLineMoves(square, 1, -1, limit_to_one, &moves);
  AddLineMoves(square, -1, -1, limit_to_one, &moves);
  AddLineMoves(square, -1, 1, limit_to_one, &moves);
  
  return moves;
}

set<Move> MoveGen::GetOrthogonalMoves(Square square, bool limit_to_one) {
  set<Move> moves;
  
  AddLineMoves(square, 1, 0, limit_to_one, &moves);
  AddLineMoves(square, 0, -1, limit_to_one, &moves);
  AddLineMoves(square, -1, 0, limit_to_one, &moves);
  AddLineMoves(square, 0, 1, limit_to_one, &moves);
  
  return moves;
}

void MoveGen::AddLineMoves(Square square, int rank_increment, int file_increment,
                           bool limit_to_one, set<Move> *moves) {
  Color color = position.GetActiveColor();
  Square dest_square = square;
  bool search_onward = true;
  while (search_onward) {
    dest_square = Square(dest_square.rank + rank_increment,
                         dest_square.file + file_increment);
    if (IsValidDestSquare(dest_square, color)) {
      moves->insert(Move(square, dest_square, position));
      if (IsCaptureSquare(dest_square, color)) {
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

Square MoveGen::LineEndingSquare(Position after_move, Square start_square,
                                 int rank_increment, int file_increment) {
  Square next_square = Square(start_square.rank + rank_increment,
    start_square.file + file_increment);
  while (next_square.is_real_square && after_move.ContentsAt(next_square) == EMPTY) {
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

bool MoveGen::IsValidDestSquare(Square square, Color color) {
  return square.is_real_square &&
    ColorOfContents(position.ContentsAt(square)) != color;
}

bool MoveGen::IsCaptureSquare(Square square, Color color) {
    if (!square.is_real_square) {
    return false;
  } else if (OppositeColors(color, ColorOfContents(position.ContentsAt(square)))) {
    return true;
  } else {
    return false;
  }
}

bool MoveGen::IsPawnCaptureSquare(Square square, Color color) {
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

set<Move> MoveGen::GetPromotionMoves(Square start_square, Square end_square) {
  set<Move> promotions;
  
  promotions.insert(Move(start_square, end_square, position, KNIGHT));
  promotions.insert(Move(start_square, end_square, position, BISHOP));
  promotions.insert(Move(start_square, end_square, position, ROOK));
  promotions.insert(Move(start_square, end_square, position, QUEEN));
  
  return promotions;
}

}
