#ifndef INTEGRATION_TESTS_H__
#define INTEGRATION_TESTS_H__

#include "coordinates.h"

#include <string>
#include <vector>
#include <set>

namespace test {

struct LegalMoveTest {
  std::string test_name;
  std::string fen;
  std::vector<std::string> moves;
  std::set<std::string> legal_moves;
};

void RunIntegrationTests();

void PerformTest(LegalMoveTest test);

}

#endif
