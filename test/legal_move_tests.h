#ifndef LEGAL_MOVE_TESTS_H__
#define LEGAL_MOVE_TESTS_H__

#include <string>
#include <vector>
#include <set>

namespace test {

struct LegalMoveTest {
  std::string test_name;
  std::string fen;
  std::vector<std::string> moves;
  std::set<std::string> legal_moves;

  LegalMoveTest() {}
  LegalMoveTest(std::string name) : test_name(name) {}
};

struct LegalMoveTestResult {
  int correct = 0;
  int omissions = 0;
  int false_positives = 0;
};

bool RunLegalMoveTests();

LegalMoveTestResult PerformTest(LegalMoveTest test);

}

#endif
