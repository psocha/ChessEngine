#ifndef POSITION_TESTS_H__
#define POSITION_TESTS_H__

namespace test {

bool RunAllPositionTests();

void TestSimpleSquaresAndMoves();
void TestCastlingPerformance();
void TestCastlingCancellation();
void TestEnPassant();
void TestPromotion();
void TestChecks();
void TestUndoMove();

}

#endif
