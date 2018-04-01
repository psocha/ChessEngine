#ifndef POSITION_H__
#define POSITION_H__

#include "pieces.h"
#include "square.h"

#include <vector>

namespace core {

struct Move;

struct CastlesAllowed {
  bool white_kingside = true;
  bool white_queenside = true;
  bool black_kingside = true;
  bool black_queenside = true;
};

bool operator==(const CastlesAllowed& left, const CastlesAllowed& right);
bool operator!=(const CastlesAllowed& left, const CastlesAllowed& right);

struct HistoryData {
  std::string move_string;
  Square last_start_square;
  Square last_end_square;
  SquareContents last_dest_square_contents;

  Square white_king_location_cache;
  Square black_king_location_cache;

  CastlesAllowed last_castles_allowed;
  Square last_en_passant_square;
  bool was_promotion;
};

bool operator==(const HistoryData& left, const HistoryData& right);
bool operator!=(const HistoryData& left, const HistoryData& right);

class Position {
public:
  Position();
  ~Position();

  void Print() const;

  void LoadFromFen(std::string fen);
  std::string Serialize() const;

  bool IsThreeFold() const;

  void PerformMove(const Move& move);
  void PerformMove(std::string move);
  void UndoLastMove();

  Color GetActiveColor() const {
    return active_color;
  }
  void SetActiveColor(std::string color_marker) {
    active_color = ColorFromChar(color_marker[0]);
  }

  CastlesAllowed GetCastle() const {
    return castles_allowed;
  }
  void SetCastle(std::string castle);
  std::string MakeCastleString() const;

  Square GetEnPassant() const {
    return en_passant_square;
  }
  void SetEnPassant(std::string en_passant) {
    en_passant_square = Square(en_passant);
  }

  inline SquareContents ContentsAt(int rank, int file) const {
    return chessboard[12*(rank + 2) + file + 2];
  }
  inline SquareContents ContentsAt(const Square& square) const {
    return chessboard[12*(square.rank + 2) + square.file + 2];
  }

  Square FindKing(Color color) const;

  bool IsCheck(Color color) const;

  bool Equals(const Position& other, bool compare_stacks) const;

private:
  SquareContents chessboard[144];

  Color active_color;
  CastlesAllowed castles_allowed;
  Square en_passant_square;

  Square white_king_location_cache;
  Square black_king_location_cache;

  std::vector<HistoryData> move_stack;

  void InitializeEmptyBoard();

  inline void SetContentsAt(int rank, int file, SquareContents contents) {
    chessboard[12*(rank + 2) + file + 2] = contents;
  }
};

}

#endif
