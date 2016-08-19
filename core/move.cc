#include "move.h"
#include "position.h"
using std::string;

namespace core {

Move::Move(string coordinates, int moving_piece) {
  start_square = Square(coordinates.substr(0, 2));
  end_square = Square(coordinates.substr(2, 2));

  moving_piece = moving_piece;

  if (coordinates.length() == 5) {
    char promotion = coordinates.at(4);
    switch (promotion) {
    case 'q':
      promoted_piece = QUEEN_W;
      break;
    case 'r':
      promoted_piece = ROOK_W;
      break;
    case 'b':
      promoted_piece = BISHOP_W;
      break;
    case 'n':
      promoted_piece = KNIGHT_W;
      break;
    default:
      promoted_piece = 0;
    }
  }

}

string Move::ToString() {
  string move_string = start_square.ToString() + end_square.ToString();
  if (promoted_piece) {
    char piece;
    switch (promoted_piece) {
    case QUEEN_W:
      piece = 'q';
      break;
    case ROOK_W:
      piece = 'r';
      break;
    case BISHOP_W:
      piece = 'b';
      break;
    case KNIGHT_W:
      piece = 'n';
      break;
    }
    move_string += piece;
  }
  return move_string;
}

}
