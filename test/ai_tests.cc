#include "ai_tests.h"

#include "test_main.h"
#include "../ai/material_positional_ai.h"
#include "../core/board.h"

#include <iostream>
using core::Board;
using std::string;

namespace test {

bool RunAITests() {
  try {
    TestObviousMateInOne();
    TestRollingRook();
    TestSacrificeMateCombo();
    TestRescueStalemate();
    TestWinPieceWithCheck();
  } catch (FailingTestException fail) {
    std::cout << fail.GetMessage() << std::endl;
    return false;
  }
  
  return true;
}

void TestObviousMateInOne() {
  Board board = TestHelper::CreateBoardFromFen("1Q6/5pk1/2p3p1/1p2N2p/1b5P/1bn5/5rP1/2K5 b - - 0 1");
  ai::MaterialPositionalAI ai;
  ai.SetDepth(3);
  
  string suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "f2c2", "Black plays mate in 1");
  
  ai.SetDepth(1);
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "f2c2", "Black plays mate in 1 even at depth 1");
}

void TestRollingRook() {
  Board board = TestHelper::CreateBoardFromFen("4k3/8/8/8/r7/7r/4K3/8 b - - 0 1");
  
  ai::MaterialPositionalAI ai;
  ai.SetDepth(3);
  
  string suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "a4a2", "Black sets up rolling rook mate");
  
  board.LoadMove("a4a2");
  board.LoadMove("e2e1");
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "h3h1", "Black completes rolling rook mate");
  
  ai.SetDepth(1);
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "h3h1", "Black completes rolling rook mate even at depth 1");
}

void TestSacrificeMateCombo() {
  Board board = TestHelper::CreateBoardFromFen("r1bk2nr/p2p1pNp/n2B4/1p1NP2P/6P1/3P1Q2/P1P1K3/q5b1 w - - 0 1");
  
  ai::MaterialPositionalAI ai;
  ai.SetDepth(3);
  
  string suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "f3f6", "White sacrifices queen to set up mate");
  
  board.LoadMove("f3f6");
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertTrue(suggested_move == "g8f6" || suggested_move == "g8e7", "Black escapes from check");
  
  board.LoadMove("g8f6");
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "d6e7", "White completes the mate");
}

void TestRescueStalemate() {
  Board board = TestHelper::CreateBoardFromFen("1r5k/7p/2n5/3r4/K3n3/7Q/8/2b5 w - - 0 1");
  
  ai::MaterialPositionalAI ai;
  ai.SetDepth(2);
  
  string suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "h3h7", "White sacrifices to get stalemate in lost position");
  
  ai.SetDepth(4);
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "h3h7", "White not confused by stalemate in chain");
}

void TestWinPieceWithCheck() {
  Board board = TestHelper::CreateBoardFromFen("2kr4/8/7b/2K5/8/8/8/6R1 b - - 0 1");
  
  ai::MaterialPositionalAI ai;
  ai.SetDepth(3);
  
  string suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "h6e3", "Black forks king and rook");
  
  board.LoadMove("h6e3");
  board.LoadMove("c5c4");
  suggested_move = ai.BestMove(board.GetPositionRef());
  TestHelper::AssertEqual(suggested_move, "e3g1", "Black wins rook");
}

}
