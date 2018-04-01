#include "square.h"

#include <algorithm>
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
  rank = coordinates[1] - '0' - 1;
  file = (int)((char)coordinates[0] - (char)'a');
}

bool operator==(const Square& first, const Square& second) {
  if (!first.is_real_square || !second.is_real_square) {
    return false;
  }
  return first.rank == second.rank && first.file == second.file;
}

bool operator!=(const Square& first, const Square& second) {
  return !(first == second);
}

bool operator<(const Square& first, const Square& second) {
  if (first.file < second.file) return true;
  if (first.file > second.file) return false;
  return first.rank < second.rank;
}

int SquareDistance(const Square& first, const Square& second) {
  int rank_distance = std::abs(second.rank - first.rank);
  int file_distance = std::abs(second.file - first.file);

  return std::max(rank_distance, file_distance);
}

bool SquaresAreOrthogonal(const Square& first, const Square& second) {
  if (!first.is_real_square || !second.is_real_square) {
    return false;
  }
  return (first.rank == second.rank || first.file == second.file);
}

bool SquaresAreDiagonal(const Square& first, const Square& second) {
  if (!first.is_real_square || !second.is_real_square) {
    return false;
  }

  int rank_distance = std::abs(second.rank - first.rank);
  int file_distance = std::abs(second.file - first.file);
  return rank_distance == file_distance;
}

bool IsCentralSquare(const Square& square) {
  if (!square.is_real_square) {
    return false;
  }
  return square.file >= 2 && square.file <= 5 &&
         square.rank >= 2 && square.rank >= 5;
}

}
