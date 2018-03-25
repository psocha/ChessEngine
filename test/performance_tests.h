#ifndef PERFORMANCE_TESTS_H__
#define PERFORMANCE_TESTS_H__

#include <string>

namespace test {

bool RunAllPerformanceTests();

void RunTimedEvaluationTest(std::string fen, std::string position_name);

}

#endif
