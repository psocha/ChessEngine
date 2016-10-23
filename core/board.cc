#include "board.h"
#include "position.h"
#include "movegen.h"
using std::set;
using std::string;

namespace core {

Board::Board() {
  position = new Position();
}

Board::~Board() {
  delete position;
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
  position->PerformMove(move);
}

std::set<string> Board::GetLegalMoves() {
  set<Move> legal_moves = MoveGen::AllLegalMoves(*position);
  set<string> moves_as_strings;

  for (Move move : legal_moves) {
    moves_as_strings.insert(move.ToString());
  }

  return moves_as_strings;
}

Position Board::GetPosition() {
  return Position(*(this->position));
}

void Board::Print() {
  this->position->Print();
}

}
