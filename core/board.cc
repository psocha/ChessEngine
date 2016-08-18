#include "board.h"
#include "position.h"
#include "movegen.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
using std::set;
using std::string;

namespace core {

Board::Board() {
  position = new Position();
  move_generator = new MoveGen();

  srand(time(NULL));
}

Board::~Board() {
  delete position;
  delete move_generator;
}

void Board::LoadStartPos() {
  LoadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
}

void Board::LoadFromFen(string fen_position, string active_color, string castle,
  string en_passant, string halfmove_clock, string fullmove_number) {
  position->LoadFromFen(fen_position);
  position->SetActiveColor(active_color);
  position->SetCastle(castle);
  position->SetEnPassant(en_passant);
}

void Board::LoadMove(string move) {

}

std::set<string> Board::GetLegalMoves() {
  set<Move> legal_moves = move_generator->AllLegalMoves(position);
  set<string> moves_as_strings;

  for (Move move : legal_moves) {
    moves_as_strings.insert(move.ToString());
  }

  return moves_as_strings;
}

string Board::BestMove() {
  set<string> legal_moves = GetLegalMoves();
  int random_index = rand() % legal_moves.size();

  set<string>::const_iterator it(legal_moves.begin());
  advance(it, random_index);

  return *it;
}

}

