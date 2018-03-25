#include "performance_tests.h"
#include "test_main.h"
#include "../ai/material_positional_ai.h"
#include "../core/board.h"

#include <chrono>
#include <iostream>
#include <string>

using core::Board;
using std::string;

namespace test {

bool RunAllPerformanceTests() {
  unsigned long starting_timestamp =
    std::chrono::system_clock::now().time_since_epoch() / 
    std::chrono::milliseconds(1);

  RunTimedEvaluationTest(STARTPOS, "Starting Position");
  RunTimedEvaluationTest("r7/1q4k1/8/8/3K4/4R3/8/8 b - - 0 1", "Q + R vs R");
  RunTimedEvaluationTest("6k1/R1p2pp1/2bp1n1p/8/2PP4/4BN2/5PPP/1r4K1 w - - 0 1", "Blocking Back Rank Mate");
  RunTimedEvaluationTest("r3k2r/p1Nn1p2/b1n1q1p1/4P2p/3P4/6Q1/PPP2PP1/2KR3R b kq - 0 1", "Family Fork");

  unsigned long ending_timestamp =
    std::chrono::system_clock::now().time_since_epoch() / 
    std::chrono::milliseconds(1);

  int duration_ms = ending_timestamp - starting_timestamp;

  std::cout << "TOTAL TESTING TIME: " << duration_ms << "ms" << std::endl;

  return true;
}

void RunTimedEvaluationTest(string fen, string position_name) {
  Board board = TestHelper::CreateBoardFromFen(fen);
  ai::MaterialPositionalAI ai;
  ai.SetSuppressLogs(true);

  std::cout << "TESTING " << position_name << std::endl;
  
  for (int depth = 3; depth <= 6; depth++) {
      ai.SetDepth(depth);

      unsigned long starting_timestamp =
        std::chrono::system_clock::now().time_since_epoch() / 
        std::chrono::milliseconds(1);

      string suggested_move = ai.BestMove(board.GetPositionRef());

      unsigned long ending_timestamp =
        std::chrono::system_clock::now().time_since_epoch() / 
        std::chrono::milliseconds(1);

      int duration_ms = ending_timestamp - starting_timestamp;

      std::cout << "Depth " << depth << ": " << suggested_move << " in " << duration_ms << "ms" << std::endl;
  }
  
  std::cout << std::endl;
}

}
