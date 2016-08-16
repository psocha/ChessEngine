#include "integration_tests.h"
#include "board.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

const string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

namespace test {

void RunIntegrationTests() {
  vector<LegalMoveTest> tests;

  LegalMoveTest start_pos_test;
  start_pos_test.test_name = "STARTING POSITION";
  start_pos_test.fen = STARTPOS;
  start_pos_test.legal_moves = { "a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3", "d2d4",
    "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4", "h2h3", "h2h4", "b1a3", "b1c3", "g1f3", "g1h3" };
  tests.push_back(start_pos_test);

  for (LegalMoveTest current_test : tests) {
    PerformTest(current_test);
  }
}

void PerformTest(LegalMoveTest test) {
  cout << "Testing " << test.test_name << endl;

  Board board;

  istringstream fen_parser(test.fen);
  string position, active_color, castle, en_passant, halfmove_clock, fullmove_number;
  fen_parser >> position;
  fen_parser >> active_color;
  fen_parser >> castle;
  fen_parser >> en_passant;
  fen_parser >> halfmove_clock;
  fen_parser >> fullmove_number;

  board.LoadFromFen(position, active_color, castle, en_passant, halfmove_clock, fullmove_number);

  for (string move : test.moves) {
    board.LoadMove(move);
  }

  set<string> test_moves = board.GetLegalMoves();
  set<string> legal_but_omitted;
  set<string> illegal_but_listed;

  set_difference(test.legal_moves.begin(), test.legal_moves.end(),
    test_moves.begin(), test_moves.end(),
    inserter(legal_but_omitted, legal_but_omitted.end()));
  set_difference(test_moves.begin(), test_moves.end(),
    test.legal_moves.begin(), test.legal_moves.end(),
    inserter(illegal_but_listed, illegal_but_listed.end()));

  bool insufficient = legal_but_omitted.size() > 0;
  bool excess = illegal_but_listed.size() > 0;

  if (insufficient) {
    cerr << "The following moves are legal but were not listed: ";
    for (string move : legal_but_omitted) {
      cerr << move << " ";
    }
    cerr << endl;
  }
  if (excess) {
    cerr << "The following moves are illegal but were listed: ";
    for (string move : illegal_but_listed) {
      cerr << move << " ";
    }
    cerr << endl;
  }

  if (!insufficient && !excess) {
    cout << "PASS" << endl;
  }
}

}
