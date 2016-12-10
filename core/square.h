#ifndef SQUARE_H__
#define SQUARE_H__

#include <string>

namespace core {

struct Square {
  bool is_real_square = true;

  int rank;
  int file;

  Square();
  Square(int rank, int file);
  Square(std::string coordinates);
  std::string ToString() const;
};

bool operator==(const Square& first, const Square& second);
bool operator!=(const Square& first, const Square& second);

int SquareDistance(Square first, Square second);
bool SquaresAreOrthogonal(Square first, Square second);
bool SquaresAreDiagonal(Square first, Square second);

}

#endif
