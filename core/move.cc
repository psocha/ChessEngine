#include "move.h"

using std::string;

namespace core {

Move::Move(string coordinates, const Position& initial_position) {
  start_square = Square(coordinates.substr(0, 2));
  end_square = Square(coordinates.substr(2, 2));

  if (coordinates.length() == 5) {
    char promotion = coordinates[4];
    switch (promotion) {
    case 'q':
      promoted_piece = QUEEN;
      break;
    case 'r':
      promoted_piece = ROOK;
      break;
    case 'b':
      promoted_piece = BISHOP;
      break;
    case 'n':
      promoted_piece = KNIGHT;
      break;
    default:
      promoted_piece = NULL_PIECE;
    }
  }

  DescribeFromPosition(initial_position);
}

Move::Move(Square start, Square end, const Position &initial_position) {
  start_square = start;
  end_square = end;

  DescribeFromPosition(initial_position);
}

Move::Move(Square start, Square end, const Position &initial_position, PieceType promotion) {
  start_square = start;
  end_square = end;

  promoted_piece = promotion;

  DescribeFromPosition(initial_position);
}

void Move::DescribeFromPosition(const Position &initial_position) {
  moving_piece = GetPieceType(initial_position.ContentsAt(start_square));

  Color start_color = ColorOfContents(initial_position.ContentsAt(start_square));
  Color end_color = ColorOfContents(initial_position.ContentsAt(end_square));
  if (OppositeColors(start_color, end_color)) {
    is_capture = true;
  }

  if (moving_piece == KING) {
    if (start_color == WHITE && start_square == Square(0, 4) &&  // e1
       (end_square == Square(0, 6) || end_square == Square(0, 2))) {  // g1 or c1
      is_castle = true;
    } else if (start_color == BLACK && start_square == Square(7, 4) &&  // e8
       (end_square == Square(7, 6) || end_square == Square(7, 2))) {  // g8 or c8
      is_castle = true;
    }
  }
}

string Move::ToString() const {
  string move_string = start_square.ToString() + end_square.ToString();
  if (promoted_piece) {
    char piece;
    switch (promoted_piece) {
      case QUEEN:
        piece = 'q';
        break;
      case ROOK:
        piece = 'r';
        break;
      case BISHOP:
        piece = 'b';
        break;
      case KNIGHT:
        piece = 'n';
        break;
      default:
        piece = 0;
    }
    move_string += piece;
  }
  return move_string;
}

bool operator==(const Move& first, const Move& second) {
  return first.start_square == second.start_square &&
         first.end_square == second.end_square &&
         first.promoted_piece == second.promoted_piece;
}

bool operator<(const Move& first, const Move& second) {
  if (first.start_square < second.start_square) return true;
  if (second.start_square < first.start_square) return false;
  return first.end_square < second.end_square;
}

}
