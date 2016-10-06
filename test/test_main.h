#ifndef TEST_MAIN_H__
#define TEST_MAIN_H__

#include "../core/Board.h"

#include <string>

namespace test {
  
const std::string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void RunAllTests();

class FailingTestException {
 public:
  FailingTestException(std::string message);
  
  std::string GetMessage() const;
 private:
  std::string message;
};

class TestHelper {
 public:
  static core::Board CreateBoardFromFen(std::string fen);
 
  static void AssertEqual(int actual, int expected, std::string failure_message);
  static void AssertEqual(std::string actual, std::string expected, std::string failure_message);
};

}

#endif
