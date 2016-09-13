#ifndef POSITION_H__
#define POSITION_H__

#include "square.h"

#include <map>
#include <vector>

namespace core {

const int EMPTY = 0;

const int PAWN =   1;
const int KNIGHT = 2;
const int BISHOP = 3;
const int ROOK =   4;
const int QUEEN =  5;
const int KING =   6;
    
const int PAWN_W =   1;
const int KNIGHT_W = 2;
const int BISHOP_W = 3;
const int ROOK_W =   4;
const int QUEEN_W =  5;
const int KING_W =   6;

const int PAWN_B =   -1;
const int KNIGHT_B = -2;
const int BISHOP_B = -3;
const int ROOK_B =   -4;
const int QUEEN_B =  -5;
const int KING_B =   -6;

const int OUT_OF_BOUNDS = 99;

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
  
  int PieceAt(Square square) const;

private:
  std::vector<std::vector<int>> chessboard;
  char active_color;
  CastlesAllowed castles_allowed;
  Square en_passant_square;

  std::map<int, char> pieceRepresentations;

  void InitializeOutOfBounds();
  void InitializePieceRepresentations();
  int PieceFromChar(char piece);
};

int PieceType(int piece);

}

#endif
