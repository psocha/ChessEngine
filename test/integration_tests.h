#ifndef INTEGRATION_TESTS_H__
#define INTEGRATION_TESTS_H__

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

void RunIntegrationTests();

void PerformTest(LegalMoveTest test);

}

#endif
