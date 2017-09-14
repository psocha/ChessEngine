#ifndef MOVE_STACK_TESTS_H__
#define MOVE_STACK_TESTS_H__

#include <string>
#include <vector>

namespace test {

struct MoveStackTest {
  std::string test_name;
  std::string starting_fen;
  std::string expected_ending_fen;
  std::vector<std::string> moves;

  MoveStackTest(std::string name) : test_name(name) {}
};

bool RunAllMoveStackTests();

bool PerformMoveStackTest(MoveStackTest test);

}

#endif
