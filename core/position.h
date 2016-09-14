#ifndef POSITION_H__
#define POSITION_H__

#include "pieces.h"
#include "square.h"

#include <map>
#include <vector>

namespace core {

struct CastlesAllowed {
  bool whiteKingside = true;
  bool whiteQueenside = true;
  bool blackKingside = true;
  bool blackQueenside = true;
};

class Position {
public:
  Position();
  ~Position();

  void Print();

  void LoadFromFen(std::string fen);
  
  void PerformMove(std::string move);
  
  void SetActiveColor(std::string active_color);
  void SetCastle(std::string castle);
  
  void SetEnPassant(std::string en_passant);
  Square GetEnPassant() const;
  
  SquareContents PieceAt(Square square) const;

private:
  std::vector<std::vector<SquareContents>> chessboard;
  Color active_color;
  CastlesAllowed castles_allowed;
  Square en_passant_square;

  void InitializeEmptyBoard();
};

}

#endif
