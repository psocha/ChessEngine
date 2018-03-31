#ifndef POSITION_H__
#define POSITION_H__

#include "pieces.h"
#include "square.h"

#include <map>
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

  std::string serialized_form;
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
  std::string GetSerialization() const {
    return serialized_form;
  }
  bool IsThreeFold() const {
    return is_three_fold;
  }

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

  std::string serialized_form;

  std::vector<HistoryData> move_stack;

  std::map<std::string, int> past_position_counts;
  bool is_three_fold;

  void InitializeEmptyBoard();
  void Serialize();
};

}

#endif
