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
  RunTimedEvaluationTest(STARTPOS, "Starting Position");

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
