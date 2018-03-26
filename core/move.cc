#include "move.h"

#include <set>
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
  } else if (SameColors(start_color, end_color)) {
    is_illegal_collision = true;
  }

  if (moving_piece == KING &&
     (start_square.ToString() == "e1" || start_square.ToString() == "e8")) {
    std::set<string> castle_moves = {"e1g1", "e1c1", "e8g8", "e8c8"};
    if (castle_moves.count(this->ToString()) > 0) {
        is_castle = true;
    }
  }

  if (moving_piece == PAWN && end_square == initial_position.GetEnPassant()) {
    is_en_passant = true;
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
  return first.ToString() == second.ToString();
}

bool operator<(const Move& first, const Move& second) {
  if (first.start_square < second.start_square) return true;
  if (second.start_square < first.start_square) return false;
  return first.end_square < second.end_square;
}

}
