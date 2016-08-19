#include "square.h"

#include <cstdlib>
#include <string>
using std::string;

namespace core {

Square::Square() {}

Square::Square(string coordinates) {
  if (coordinates == "-") {
    is_real_square = false;
    return;
  }

  is_real_square = true;
  rank = coordinates[1] - '0';
  file = (int)((char)coordinates[0] - (char)'a');
}

string Square::ToString() {
  if (!is_real_square) {
    return "-";
  }

  string square_representation;
  square_representation += std::to_string(file + (int)'a');
  square_representation += std::to_string(rank);

  return square_representation;
}

}
