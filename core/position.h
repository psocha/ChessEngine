#ifndef POSITION_H__
#define POSITION_H__

#include "pieces.h"
#include "square.h"

#include <map>
#include <vector>

namespace core {

struct CastlesAllowed {
  bool white_kingside = true;
  bool white_queenside = true;
  bool black_kingside = true;
  bool black_queenside = true;
};

bool operator==(const CastlesAllowed& left, const CastlesAllowed& right);
bool operator!=(const CastlesAllowed& left, const CastlesAllowed& right);

struct HistoryData {
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

  void PerformMove(std::string move);
  void UndoLastMove();

  void SetActiveColor(std::string color_marker);
  Color GetActiveColor() const;

  void SetCastle(std::string castle);
  CastlesAllowed GetCastle() const;
  std::string MakeCastleString() const;

  void SetEnPassant(std::string en_passant);
  Square GetEnPassant() const;

  inline SquareContents ContentsAt(Square square) const {
    return chessboard[square.rank + 2][square.file + 2];
  }

  Square FindKing(Color color) const;

  bool IsCheck(Color color) const;

  bool Equals(const Position& other, bool compare_stacks) const;

private:
  std::vector<std::vector<SquareContents>> chessboard;
  Color active_color;
  CastlesAllowed castles_allowed;
  Square en_passant_square;

  Square white_king_location_cache;
  Square black_king_location_cache;

  std::vector<HistoryData> move_stack;

  void InitializeEmptyBoard();
};

}

#endif
