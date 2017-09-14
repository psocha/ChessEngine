#include "move_stack_tests.h"
#include "test_main.h"

#include <iostream>
#include <vector>

using core::Board;
using core::Position;
using std::string;
using std::vector;

namespace test {

bool RunAllMoveStackTests() {
  std::vector<MoveStackTest> stack_tests;

  MoveStackTest center_counter_game_test("center counter game");
  center_counter_game_test.starting_fen = STARTPOS;
  center_counter_game_test.moves = { "e2e4", "d7d5", "e4d5", "d8d5", "b1c3", "d5a5", "f1c4", "e7e5",
    "c4f7", "e8f7", "h2h4" };
  center_counter_game_test.expected_ending_fen = "rnb2bnr/ppp2kpp/8/q3p3/7P/2N5/PPPP1PP1/R1BQK1NR b KQ h3 0 1";
  stack_tests.push_back(center_counter_game_test);

  MoveStackTest castling_test("castling");
  castling_test.starting_fen = "r3k2r/5ppp/p3p3/3p4/P2PP3/5P2/6PP/R3K2R w KQkq - 0 1";
  castling_test.moves = { "e1g1", "d5e4", "f3e4", "a8c8", "a1c1", "e8e7" };
  castling_test.expected_ending_fen = "2r4r/4kppp/p3p3/8/P2PP3/8/6PP/2R2RK1 w - - 0 1";
  stack_tests.push_back(castling_test);

  MoveStackTest promotion_test("promotion");
  promotion_test.starting_fen = "8/PP6/1K6/8/8/5kp1/7p/6N1 b - - 0 1";
  promotion_test.moves = { "h2g1q", "b7b8b", "g1b1", "b6a6", "b1b8", "a7b8r", "g3g2" };
  promotion_test.expected_ending_fen = "1R6/8/K7/8/8/5k2/6p1/8 w - - 0 1";
  stack_tests.push_back(promotion_test);

  MoveStackTest en_passant_test("en passant");
  en_passant_test.starting_fen = "2k5/1ppp1p2/p7/2P1P3/3Pp3/8/5PPP/6K1 b - d3 0 1";
  en_passant_test.moves = { "e4d3", "g1f1", "d3d2", "f1e2", "d7d5", "c5d6", "f7f5", "e5f6", "c7d6", "e2d2" };
  en_passant_test.expected_ending_fen = "2k5/1p6/p2p1P2/8/8/8/3K1PPP/8 b - - 0 1";
  stack_tests.push_back(en_passant_test);

  int total_tests = stack_tests.size();
  int passed_tests = 0;
  for (MoveStackTest test : stack_tests) {
    bool passed = PerformMoveStackTest(test);
    if (passed) passed_tests++;
  }

  std::cout << passed_tests << " of " << total_tests << " move stack tests passed." << std::endl;
  return passed_tests == total_tests;
}

bool PerformMoveStackTest(MoveStackTest test) {
  vector<Position> saved_positions;
  Board board = TestHelper::CreateBoardFromFen(test.starting_fen);
  saved_positions.push_back(board.GetPosition());

  for (string move : test.moves) {
    board.LoadMove(move);
    saved_positions.push_back(board.GetPosition());
  }

  Board end_board = TestHelper::CreateBoardFromFen(test.expected_ending_fen);
  if (!end_board.GetPosition().Equals(board.GetPosition(), false)) {
    std::cout << "Boards at end of " << test.test_name << " don't match." << std::endl;
    return false;
  }

  for (int i = saved_positions.size() - 2; i >= 0; i--) {
    board.UndoLastMove();
    if (!board.GetPosition().Equals(saved_positions[i], true)) {
      std::cout << "Reverted position not equal on ply " << i << " of position " << test.test_name << std::endl;
      return false;
    }
  }
  return true;
}

}
