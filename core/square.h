#ifndef SQUARE_H__
#define SQUARE_H__

#include <string>

namespace core {

struct Square {
  bool is_real_square = true;

  int rank;
  int file;

  Square();

  inline Square(int rank, int file) {
    is_real_square = rank >= 0 && rank <= 7 && file >= 0 && file <= 7;
    this->rank = rank;
    this->file = file;
  }

  Square(std::string coordinates);

  inline std::string ToString() const {
    if (!is_real_square) {
      return "-";
    }

    std::string square_representation;
    square_representation += (char)(file + (int)'a');
    square_representation += (char)(rank + 1 + (int)'0');

    return square_representation;
  }

};

bool operator==(const Square& first, const Square& second);
bool operator!=(const Square& first, const Square& second);
bool operator<(const Square& first, const Square& second);

int SquareDistance(Square first, Square second);
bool SquaresAreOrthogonal(Square first, Square second);
bool SquaresAreDiagonal(Square first, Square second);

}

#endif
