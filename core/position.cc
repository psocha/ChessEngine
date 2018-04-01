#include "position.h"
#include "move.h"
#include "movegen.h"

#include <iostream>

using std::map;
using std::string;
using std::vector;

namespace core {

bool operator==(const CastlesAllowed& left, const CastlesAllowed& right) {
  return left.white_kingside == right.white_kingside &&
         left.white_queenside == right.white_queenside &&
         left.black_kingside == right.black_kingside &&
         left.black_queenside == right.black_queenside;
}

bool operator!=(const CastlesAllowed& left, const CastlesAllowed& right) {
  return !(left == right);
}

bool operator==(const HistoryData& left, const HistoryData& right) {
  return left.move_string == right.move_string &&
         left.last_start_square == right.last_start_square &&
         left.last_end_square == right.last_end_square &&
         left.last_dest_square_contents == right.last_dest_square_contents &&
         left.white_king_location_cache == right.white_king_location_cache &&
         left.black_king_location_cache == right.black_king_location_cache &&
         left.last_castles_allowed == right.last_castles_allowed &&
         left.last_en_passant_square.ToString() == right.last_en_passant_square.ToString() &&
         left.was_promotion == right.was_promotion;
}

bool operator!=(const HistoryData& left, const HistoryData& right) {
  return !(left == right);
}

Position::Position() {
  InitializeEmptyBoard();
}

Position::~Position() {}

void Position::Print() const {
  for (int rank = 9; rank >= 2; rank--) {
    for (int file = 2; file < 10; file++) {
      SquareContents piece = ContentsAt(rank - 2, file - 2);
      char piece_char = CharFromSquareContents(piece);
      std::cout << " " << piece_char << " ";
    }
    std::cout << std::endl;
  }
  std::cout << CharFromColor(GetActiveColor()) << " ";
  std::cout << MakeCastleString() << " ";
  std::cout << GetEnPassant().ToString() << std::endl;
}

void Position::LoadFromFen(string fen) {
  InitializeEmptyBoard();

  int rank = 9;
  int file = 2;
  for (unsigned int i = 0; i < fen.length(); i++) {
    char ch = fen[i];

    if (ch == '/') {
      rank--;
      file = 2;
      continue;
    }

    if (isdigit(ch)) {
      int squares_to_skip = ch - '0';
      file += squares_to_skip;
    } else {
      SquareContents piece = SquareContentsFromChar(ch);
      SetContentsAt(rank - 2, file - 2, piece);

      if (piece == KING_W) {
        white_king_location_cache = Square(rank - 2, file - 2);
      } else if (piece == KING_B) {
        black_king_location_cache = Square(rank - 2, file - 2);
      }

      file++;
    }
  }
}

string Position::Serialize() const {
  string str = "";
  int empties = 0;

  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file <= 7; file++) {
      SquareContents contents = ContentsAt(rank, file);
      if (contents == EMPTY) {
        empties++;
      } else {
        if (empties > 0) {
          str += (char)(empties + (int)'0');
          empties = 0;
        }
        str += CharFromSquareContents(contents);
      }

      if (file == 7) {
        if (empties > 0) {
          str += (char)(empties + (int)'0');
          empties = 0;
        }
        if (rank > 0) {
          str += "/";
        }
      }
    }
  }

  str += " ";
  str += CharFromColor(GetActiveColor());
  str += " ";
  str += MakeCastleString();
  str += " ";
  str += GetEnPassant().ToString();

  return str;
}

bool Position::IsThreeFold() const {
  int move_list_size = move_stack.size();
  if (move_list_size < 8) {
    return false;
  }
  return move_stack[move_list_size - 1].move_string == move_stack[move_list_size - 5].move_string &&
         move_stack[move_list_size - 2].move_string == move_stack[move_list_size - 6].move_string &&
         move_stack[move_list_size - 3].move_string == move_stack[move_list_size - 7].move_string &&
         move_stack[move_list_size - 4].move_string == move_stack[move_list_size - 8].move_string;
}

void Position::PerformMove(const Move& move) {
  string move_str = move.ToString();

  HistoryData history_data;
  history_data.move_string = move_str;
  history_data.last_start_square = move.start_square;
  history_data.last_end_square = move.end_square;
  history_data.last_dest_square_contents = ContentsAt(move.end_square.rank, move.end_square.file);
  history_data.white_king_location_cache = this->white_king_location_cache;
  history_data.black_king_location_cache = this->black_king_location_cache;
  history_data.last_castles_allowed = GetCastle();
  history_data.last_en_passant_square = GetEnPassant();
  history_data.was_promotion = false;

  SquareContents moving_piece = ContentsAt(move.start_square);
  SetContentsAt(move.start_square.rank, move.start_square.file, EMPTY);
  SetContentsAt(move.end_square.rank, move.end_square.file, moving_piece);

  if (GetPieceType(moving_piece) == PAWN && move.promoted_piece != NULL_PIECE) {
    SetContentsAt(move.end_square.rank, move.end_square.file, MakePiece(move.promoted_piece, this->active_color));
    history_data.was_promotion = true;
  }

  if (moving_piece == KING_W) {
    castles_allowed.white_kingside = false;
    castles_allowed.white_queenside = false;
    white_king_location_cache = move.end_square;
  }
  if (moving_piece == KING_B) {
    castles_allowed.black_kingside = false;
    castles_allowed.black_queenside = false;
    black_king_location_cache = move.end_square;
  }

  if (moving_piece == KING_W && move_str == "e1g1") {
    SetContentsAt(0, 7, EMPTY);
    SetContentsAt(0, 5, ROOK_W);
  }
  if (moving_piece == KING_W && move_str == "e1c1") {
    SetContentsAt(0, 0, EMPTY);
    SetContentsAt(0, 3, ROOK_W);
  }
  if (moving_piece == KING_B && move_str == "e8g8") {
    SetContentsAt(7, 7, EMPTY);
    SetContentsAt(7, 5, ROOK_B);
  }
  if (moving_piece == KING_B && move_str == "e8c8") {
    SetContentsAt(7, 0, EMPTY);
    SetContentsAt(7, 3, ROOK_B);
  }

  if (move.start_square == Square("h1") || move.end_square == Square("h1")) {
    castles_allowed.white_kingside = false;
  }
  if (move.start_square == Square("a1") || move.end_square == Square("a1")) {
    castles_allowed.white_queenside = false;
  }
  if (move.start_square == Square("h8") || move.end_square == Square("h8")) {
    castles_allowed.black_kingside = false;
  }
  if (move.start_square == Square("a8") || move.end_square == Square("a8")) {
    castles_allowed.black_queenside = false;
  }

  if (GetPieceType(moving_piece) == PAWN && move.end_square == this->en_passant_square) {
    if (active_color == WHITE) {
      SetContentsAt(en_passant_square.rank - 1, en_passant_square.file, EMPTY);
    } else {
      SetContentsAt(en_passant_square.rank + 1, en_passant_square.file, EMPTY);
    }
  }

  SetEnPassant("-");

  if (moving_piece == PAWN_W && move.start_square.rank == 1 && move.end_square.rank == 3) {
    en_passant_square = Square(move.start_square.rank + 1, move.start_square.file);
  }
  if (moving_piece == PAWN_B && move.start_square.rank == 6 && move.end_square.rank == 4) {
    en_passant_square = Square(move.start_square.rank - 1, move.start_square.file);
  }

  if (ColorOfContents(moving_piece) == WHITE) {
    active_color = BLACK;
  } else {
    active_color = WHITE;
  }

  move_stack.push_back(history_data);
}

void Position::PerformMove(std::string mv) {
  Move move = Move(mv, *this);
  PerformMove(move);
}

void Position::UndoLastMove() {
  HistoryData history_data = move_stack.back();
  move_stack.pop_back();

  if (active_color == BLACK && ContentsAt(history_data.last_end_square) == PAWN_W &&
      history_data.last_end_square == history_data.last_en_passant_square) {
    SetContentsAt(history_data.last_end_square.rank - 1, history_data.last_end_square.file, PAWN_B);
  }
  if (active_color == WHITE && ContentsAt(history_data.last_end_square) == PAWN_B &&
      history_data.last_end_square == history_data.last_en_passant_square) {
    SetContentsAt(history_data.last_end_square.rank + 1, history_data.last_end_square.file, PAWN_W);
  }
  en_passant_square = history_data.last_en_passant_square;

  white_king_location_cache = history_data.white_king_location_cache;
  black_king_location_cache = history_data.black_king_location_cache;

  SetContentsAt(history_data.last_start_square.rank, history_data.last_start_square.file,
      ContentsAt(history_data.last_end_square.rank, history_data.last_end_square.file));

  SetContentsAt(history_data.last_end_square.rank, history_data.last_end_square.file,
    history_data.last_dest_square_contents);

  if (history_data.last_start_square.ToString() == "e1" &&
      history_data.last_end_square.ToString() == "g1" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("f1")) == ROOK_W) {
    SetContentsAt(0, 5, EMPTY);
    SetContentsAt(0, 7, ROOK_W);
  }
  if (history_data.last_start_square.ToString() == "e1" &&
      history_data.last_end_square.ToString() == "c1" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("d1")) == ROOK_W) {
    SetContentsAt(0, 3, EMPTY);
    SetContentsAt(0, 0, ROOK_W);
  }
  if (history_data.last_start_square.ToString() == "e8" &&
      history_data.last_end_square.ToString() == "g8" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("f8")) == ROOK_B) {
    SetContentsAt(7, 5, EMPTY);
    SetContentsAt(7, 7, ROOK_B);
  }
  if (history_data.last_start_square.ToString() == "e8" &&
      history_data.last_end_square.ToString() == "c8" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("d8")) == ROOK_B) {
    SetContentsAt(7, 3, EMPTY);
    SetContentsAt(7, 0, ROOK_B);
  }
  castles_allowed = history_data.last_castles_allowed;

  if (history_data.was_promotion) {
    if (history_data.last_end_square.rank == 7) {
      SetContentsAt(history_data.last_start_square.rank, history_data.last_start_square.file, PAWN_W);
    } else if (history_data.last_end_square.rank == 0) {
      SetContentsAt(history_data.last_start_square.rank, history_data.last_start_square.file, PAWN_B);
    }
  }

  active_color = active_color == WHITE ? BLACK : WHITE;
}

void Position::SetCastle(std::string castle) {
  castles_allowed.white_kingside = castle.find('K') != string::npos;
  castles_allowed.white_queenside = castle.find('Q') != string::npos;
  castles_allowed.black_kingside = castle.find('k') != string::npos;
  castles_allowed.black_queenside = castle.find('q') != string::npos;
}

string Position::MakeCastleString() const {
  string castles = "";
  if (castles_allowed.white_kingside) castles += "K";
  if (castles_allowed.white_queenside) castles += "Q";
  if (castles_allowed.black_kingside) castles += "k";
  if (castles_allowed.black_queenside) castles += "q";
  if (castles.length() > 0) {
    return castles;
  } else {
    return "-";
  }
}

Square Position::FindKing(Color color) const {
  if (color == WHITE) {
    return white_king_location_cache;
  } else {
    return black_king_location_cache;
  }
}

bool Position::IsCheck(Color color) const {
  vector<Square> king_squares;
  Square king_square = FindKing(color);
  king_squares.push_back(king_square);
  return MoveGen::IsInCheck(*this, king_squares, color);
}

bool Position::Equals(const Position& other, bool compare_stacks) const {
  if (GetActiveColor() != other.GetActiveColor()) {
    std::cout << "Position mismatch at active color" << std::endl;
    return false;
  }
  if (GetCastle() != other.GetCastle()) {
    std::cout << "Position mismatch at castle status" << std::endl;
    return false;
  }
  if (GetEnPassant().ToString() != other.GetEnPassant().ToString()) {
    std::cout << "Position mismatch at en passant square" << std::endl;
    return false;
  }

  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 12; j++) {
      if (chessboard[12*i + j] != other.chessboard[12*i + j]) {
        std::cout << "Position mismatch on board" << std::endl;
        return false;
      }
    }
  }

  if (compare_stacks) {
    if (move_stack.size() != other.move_stack.size()) {
      std::cout << "Position mismatch on stack size";
      return false;
    }
    for (unsigned int i = 0; i < move_stack.size(); i++) {
      if (move_stack[i] != other.move_stack[i]) {
        std::cout << "Position mismatch on stack contents at index " << i << std::endl;
        return false;
      }
    }
  }

  return true;
}

void Position::InitializeEmptyBoard() {
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 12; j++) {
      chessboard[12*i + j] = EMPTY;
    }
  }

  for (int i = 0; i < 12; i++) {
    if (i < 2 || i > 9) {
      for (int j = 0; j < 12; j++) {
        chessboard[12*i + j] = OUT_OF_BOUNDS;
      }
    }
    else {
      chessboard[12*i + 0] = OUT_OF_BOUNDS;
      chessboard[12*i + 1] = OUT_OF_BOUNDS;
      chessboard[12*i + 10] = OUT_OF_BOUNDS;
      chessboard[12*i + 11] = OUT_OF_BOUNDS;
    }
  }

  move_stack.clear();
}

}
