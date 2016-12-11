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
  return left.last_start_square == right.last_start_square &&
         left.last_end_square == right.last_end_square &&
         left.last_dest_square_contents == right.last_dest_square_contents &&
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
      SquareContents piece = chessboard[rank][file];
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
    char ch = fen.at(i);

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
      chessboard[rank][file] = piece;
      file++;
    }
  }
}

string Position::Serialize() const {
  string str = "";
  int empties = 0;
  
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file <= 7; file++) {
      SquareContents contents = chessboard.at(rank + 2).at(file + 2);
      if (contents == EMPTY) {
        empties++;
      } else {
        if (empties > 0) {
          str += std::to_string(empties);
          empties = 0;
        }
        str += CharFromSquareContents(contents);
      }
      
      if (file == 7) {
        if (empties > 0) {
          str += std::to_string(empties);
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

void Position::PerformMove(std::string mv) {
  Move move = Move(mv, *this);
  
  HistoryData history_data;
  history_data.last_start_square = move.start_square;
  history_data.last_end_square = move.end_square;
  history_data.last_dest_square_contents =
    chessboard.at(move.end_square.rank + 2).at(move.end_square.file + 2);
  history_data.last_castles_allowed = GetCastle();
  history_data.last_en_passant_square = GetEnPassant();
  history_data.was_promotion = false;
  
  SquareContents moving_piece = ContentsAt(move.start_square);
  chessboard.at(move.start_square.rank + 2).at(move.start_square.file + 2) = EMPTY;
  chessboard.at(move.end_square.rank + 2).at(move.end_square.file + 2) = moving_piece;
  
  if (GetPieceType(moving_piece) == PAWN && move.promoted_piece != NULL_PIECE) {
    chessboard.at(move.end_square.rank + 2).at(move.end_square.file + 2) =
      MakePiece(move.promoted_piece, this->active_color);
    history_data.was_promotion = true;
  }
  
  if (moving_piece == KING_W) {
    castles_allowed.white_kingside = false;
    castles_allowed.white_queenside = false;
  }
  if (moving_piece == KING_B) {
    castles_allowed.black_kingside = false;
    castles_allowed.black_queenside = false;
  }
  
  if (moving_piece == KING_W && move.ToString() == "e1g1") {
    chessboard[0 + 2][7 + 2] = EMPTY;
    chessboard[0 + 2][5 + 2] = ROOK_W;
  }
  if (moving_piece == KING_W && move.ToString() == "e1c1") {
    chessboard[0 + 2][0 + 2] = EMPTY;
    chessboard[0 + 2][3 + 2] = ROOK_W;
  }
  if (moving_piece == KING_B && move.ToString() == "e8g8") {
    chessboard[7 + 2][7 + 2] = EMPTY;
    chessboard[7 + 2][5 + 2] = ROOK_B;
  }
  if (moving_piece == KING_B && move.ToString() == "e8c8") {
    chessboard[7 + 2][0 + 2] = EMPTY;
    chessboard[7 + 2][3 + 2] = ROOK_B;
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
      chessboard[2 + en_passant_square.rank - 1][2 + en_passant_square.file] = EMPTY;
    } else {
      chessboard[2 + en_passant_square.rank + 1][2 + en_passant_square.file] = EMPTY;
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

void Position::UndoLastMove() {
  HistoryData history_data = move_stack.back();
  move_stack.pop_back();
  
  if (active_color == BLACK && ContentsAt(history_data.last_end_square) == PAWN_W &&
      history_data.last_end_square == history_data.last_en_passant_square) {
    chessboard[2 + history_data.last_end_square.rank - 1]
              [2 + history_data.last_end_square.file] = PAWN_B;
  }
  if (active_color == WHITE && ContentsAt(history_data.last_end_square) == PAWN_B &&
      history_data.last_end_square == history_data.last_en_passant_square) {
    chessboard[2 + history_data.last_end_square.rank + 1]
              [2 + history_data.last_end_square.file] = PAWN_W;
  }
  en_passant_square = history_data.last_en_passant_square;
  
  chessboard[history_data.last_start_square.rank + 2][history_data.last_start_square.file + 2] =
    chessboard[history_data.last_end_square.rank + 2][history_data.last_end_square.file + 2];

  chessboard[history_data.last_end_square.rank + 2][history_data.last_end_square.file + 2] =
    history_data.last_dest_square_contents;

  if (history_data.last_start_square.ToString() == "e1" &&
      history_data.last_end_square.ToString() == "g1" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("f1")) == ROOK_W) {
    chessboard[0 + 2][5 + 2] = EMPTY;
    chessboard[0 + 2][7 + 2] = ROOK_W;
  }
  if (history_data.last_start_square.ToString() == "e1" &&
      history_data.last_end_square.ToString() == "c1" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("d1")) == ROOK_W) {
    chessboard[0 + 2][3 + 2] = EMPTY;
    chessboard[0 + 2][0 + 2] = ROOK_W;
  }
  if (history_data.last_start_square.ToString() == "e8" &&
      history_data.last_end_square.ToString() == "g8" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("f8")) == ROOK_B) {
    chessboard[7 + 2][5 + 2] = EMPTY;
    chessboard[7 + 2][7 + 2] = ROOK_B;
  }
  if (history_data.last_start_square.ToString() == "e8" &&
      history_data.last_end_square.ToString() == "c8" &&
      history_data.last_dest_square_contents == EMPTY &&
      ContentsAt(Square("f1")) == ROOK_W) {
    chessboard[7 + 2][3 + 2] = EMPTY;
    chessboard[7 + 2][0 + 2] = ROOK_B;
  }
  castles_allowed = history_data.last_castles_allowed;
  
  if (history_data.was_promotion) {
    if (history_data.last_end_square.rank == 7) {
      chessboard[history_data.last_start_square.rank + 2]
                [history_data.last_start_square.file + 2] = PAWN_W;
    } else if (history_data.last_end_square.rank == 0) {
      chessboard[history_data.last_start_square.rank + 2]
                [history_data.last_start_square.file + 2] = PAWN_B;
    }
  }
  
  active_color = active_color == WHITE ? BLACK : WHITE;
}

void Position::SetActiveColor(std::string color_marker) {
  active_color = ColorFromChar(color_marker.at(0));
}

Color Position::GetActiveColor() const {
  return active_color;
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

CastlesAllowed Position::GetCastle() const {
  return castles_allowed;
}

void Position::SetEnPassant(std::string en_passant) {
  en_passant_square = Square(en_passant);
}

Square Position::GetEnPassant() const {
  return en_passant_square;
}

SquareContents Position::ContentsAt(Square square) const {
  return chessboard.at(square.rank + 2).at(square.file + 2);
}

Square Position::FindKing(Color color) const {
  SquareContents target = MakePiece(KING, color);
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Square square = Square(rank, file);
      if (this->ContentsAt(square) == target) {
        return square;
      }
    }
  }
  return Square("-");
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
      if (chessboard[i][j] != other.chessboard[i][j]) {
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
    chessboard.push_back(vector<SquareContents>(12));
    for (int j = 0; j < 12; j++) {
      chessboard[i][j] = EMPTY;
    }
  }

  for (int i = 0; i < 12; i++) {
    if (i < 2 || i > 9) {
      for (int j = 0; j < 12; j++) {
        chessboard[i][j] = OUT_OF_BOUNDS;
      }
    }
    else {
      chessboard[i][0] = OUT_OF_BOUNDS;
      chessboard[i][1] = OUT_OF_BOUNDS;
      chessboard[i][10] = OUT_OF_BOUNDS;
      chessboard[i][11] = OUT_OF_BOUNDS;
    }
  }
  
  move_stack.clear();
}

}
