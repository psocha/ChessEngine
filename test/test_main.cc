#include "test_main.h"
#include "position_tests.h"
#include "legal_move_tests.h"

#include <iostream>
#include <sstream>
using std::string;

namespace test {

void RunAllTests() {
  
  bool position_tests_passed = RunAllPositionTests();
  if (!position_tests_passed) {
    std::cout << "Basic position tests failed. Terminating." << std::endl;
    return;
  }
  
  bool legal_tests_passed = RunLegalMoveTests();
  if (!legal_tests_passed) {
    std::cout << "Legal move tests failed. Terminating." << std::endl;
    return;
  }
  
  std::cout << "ALL TESTS PASSING." << std::endl;
}

FailingTestException::FailingTestException(string message) : message(message) {}

string FailingTestException::GetMessage() const {
  return message;
}

core::Board TestHelper::CreateBoardFromFen(string fen) {
  core::Board board;

  std::istringstream fen_parser(fen);
  std::string position, active_color, castle, en_passant, halfmove_clock, fullmove_number;
  fen_parser >> position;
  fen_parser >> active_color;
  fen_parser >> castle;
  fen_parser >> en_passant;
  fen_parser >> halfmove_clock;
  fen_parser >> fullmove_number;

  board.LoadFromFen(position, active_color, castle, en_passant, halfmove_clock, fullmove_number);
  
  return board;
}

void TestHelper::AssertTrue(bool condition, string message) {
  if (!condition) {
    throw FailingTestException("Failure: " + message + ": assertion failed");
  }
}

void TestHelper::AssertFalse(bool condition, string message) {
  TestHelper::AssertTrue(!condition, message);
}

void TestHelper::AssertContentsAt(core::Position position, string square,
    core::SquareContents expected_contents, std::string failure_message) {
  TestHelper::AssertEqual((int)position.ContentsAt(core::Square(square)), (int)expected_contents, failure_message);
}

void TestHelper::AssertEqual(int actual, int expected, string message) {
  if (expected != actual) {
    throw FailingTestException("Failure: " + message + ": expected " +
      std::to_string(expected) + " but got " + std::to_string(actual));
  }
}

void TestHelper::AssertEqual(string actual, string expected, string message) {
  if (expected != actual) {
    throw FailingTestException("Failure: " + message + ": expected " +
      expected + " but got " + actual);
  }
}

}
