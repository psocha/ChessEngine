#include "position_tests.h"
#include "test_main.h"
#include "../core/board.h"
#include "../core/move.h"

#include <iostream>
using core::Board;
using core::Move;

namespace test {

bool RunAllPositionTests() {
  try {
    TestSimpleSquaresAndMoves();
  } catch (FailingTestException fail) {
    std::cout << fail.GetMessage() << std::endl;
    return false;
  }
  
  return true;
}

void TestSimpleSquaresAndMoves() {
  Board board = TestHelper::CreateBoardFromFen(STARTPOS);
  
  Move move = Move("e2e4", board.GetPosition());
  
  TestHelper::AssertEqual(move.ToString(), "e2e4", "move ToString()");
  TestHelper::AssertEqual(move.start_square.ToString(), "e2", "start square ToString()");
  TestHelper::AssertEqual(move.end_square.ToString(), "e4", "end square ToString()");
}

}
