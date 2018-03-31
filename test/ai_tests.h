#ifndef AI_TESTS_H__
#define AI_TESTS_H__

namespace test {

bool RunAITests();

void TestObviousMateInOne();
void TestRollingRook();
void TestSacrificeMateCombo();
void TestRescueStalemate();
void TestWinPieceWithCheck();
void TestPerpetualCheck();

}

#endif
