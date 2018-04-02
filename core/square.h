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

// Squares commonly seen in castling
const Square a1 = Square(0, 0);
const Square b1 = Square(0, 1);
const Square c1 = Square(0, 2);
const Square d1 = Square(0, 3);
const Square e1 = Square(0, 4);
const Square f1 = Square(0, 5);
const Square g1 = Square(0, 6);
const Square h1 = Square(0, 7);

const Square a8 = Square(7, 0);
const Square b8 = Square(7, 1);
const Square c8 = Square(7, 2);
const Square d8 = Square(7, 3);
const Square e8 = Square(7, 4);
const Square f8 = Square(7, 5);
const Square g8 = Square(7, 6);
const Square h8 = Square(7, 7);

bool operator==(const Square& first, const Square& second);
bool operator!=(const Square& first, const Square& second);
bool operator<(const Square& first, const Square& second);

int SquareDistance(const Square& first, const Square& second);
bool SquaresAreOrthogonal(const Square& first, const Square& second);
bool SquaresAreDiagonal(const Square& first, const Square& second);
bool IsCentralSquare(const Square& square);

}

#endif
