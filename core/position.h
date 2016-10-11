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

class Position {
public:
  Position();
  ~Position();

  void Print();

  void LoadFromFen(std::string fen);
  
  void PerformMove(std::string move);
  
  void SetActiveColor(std::string color_marker);
  Color GetActiveColor() const;
  
  void SetCastle(std::string castle);
  CastlesAllowed GetCastle() const;
  
  void SetEnPassant(std::string en_passant);
  Square GetEnPassant() const;
  
  SquareContents ContentsAt(Square square) const;

private:
  std::vector<std::vector<SquareContents>> chessboard;
  Color active_color;
  CastlesAllowed castles_allowed;
  Square en_passant_square;

  void InitializeEmptyBoard();
};

}

#endif
