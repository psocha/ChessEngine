#include "integration_tests.h"
#include "../core/board.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace test {

const std::string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void RunIntegrationTests() {
  std::vector<LegalMoveTest> tests;

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
  std::cout << "Testing " << test.test_name << std::endl;

  core::Board board;

  std::istringstream fen_parser(test.fen);
  std::string position, active_color, castle, en_passant, halfmove_clock, fullmove_number;
  fen_parser >> position;
  fen_parser >> active_color;
  fen_parser >> castle;
  fen_parser >> en_passant;
  fen_parser >> halfmove_clock;
  fen_parser >> fullmove_number;

  board.LoadFromFen(position, active_color, castle, en_passant, halfmove_clock, fullmove_number);

  for (std::string move : test.moves) {
    board.LoadMove(move);
  }

  std::set<std::string> test_moves = board.GetLegalMoves();
  std::set<std::string> legal_but_omitted;
  std::set<std::string> illegal_but_listed;

  set_difference(test.legal_moves.begin(), test.legal_moves.end(),
    test_moves.begin(), test_moves.end(),
    inserter(legal_but_omitted, legal_but_omitted.end()));
  set_difference(test_moves.begin(), test_moves.end(),
    test.legal_moves.begin(), test.legal_moves.end(),
    inserter(illegal_but_listed, illegal_but_listed.end()));

  bool insufficient = legal_but_omitted.size() > 0;
  bool excess = illegal_but_listed.size() > 0;

  if (insufficient) {
    std::cerr << "The following moves are legal but were not listed: ";
    for (std::string move : legal_but_omitted) {
      std::cerr << move << " ";
    }
    std::cerr << std::endl;
  }
  if (excess) {
    std::cerr << "The following moves are illegal but were listed: ";
    for (std::string move : illegal_but_listed) {
      std::cerr << move << " ";
    }
    std::cerr << std::endl;
  }

  if (!insufficient && !excess) {
    std::cout << "PASS" << std::endl;
  }
}

}
