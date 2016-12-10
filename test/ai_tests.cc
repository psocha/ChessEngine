#include "ai_tests.h"

#include "test_main.h"
#include "../ai/material_ai.h"
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
  } catch (FailingTestException fail) {
    std::cout << fail.GetMessage() << std::endl;
    return false;
  }
  
  return true;
}

void TestObviousMateInOne() {
  Board board = TestHelper::CreateBoardFromFen("1Q6/5pk1/2p3p1/1p2N2p/1b5P/1bn5/5rP1/2K5 b - - 0 1");
  ai::MaterialAI material_ai;
  material_ai.SetDepth(3);
  
  string suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "f2c2", "Black plays mate in 1");
  
  material_ai.SetDepth(1);
  suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "f2c2", "Black plays mate in 1 even at depth 1");
}

void TestRollingRook() {
  Board board = TestHelper::CreateBoardFromFen("4k3/8/8/8/r7/7r/4K3/8 b - - 0 1");
  
  ai::MaterialAI material_ai;
  material_ai.SetDepth(3);
  
  string suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "a4a2", "Black sets up rolling rook mate");
}

void TestSacrificeMateCombo() {
  Board board = TestHelper::CreateBoardFromFen("r1bk2nr/p2p1pNp/n2B4/1p1NP2P/6P1/3P1Q2/P1P1K3/q5b1 w - - 0 1");
  
  ai::MaterialAI material_ai;
  material_ai.SetDepth(3);
  
  string suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "f3f6", "White sacrifices queen to set up mate");
  
  board.LoadMove("f3f6");
  suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "g8f6", "Black escapes from check");
  
  board.LoadMove("g8f6");
  suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "d6e7", "White completes the mate");
}

void TestRescueStalemate() {
  Board board = TestHelper::CreateBoardFromFen("1r5k/7p/2n5/3r4/K3n3/7Q/8/2b5 w - - 0 1");
  
  ai::MaterialAI material_ai;
  material_ai.SetDepth(2);
  
  string suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "h3h7", "White sacrifices to get stalemate in lost position");
  
  material_ai.SetDepth(4);
  suggested_move = material_ai.BestMove(board.GetPosition());
  TestHelper::AssertEqual(suggested_move, "h3h7", "White not confused by stalemate in chain");
}

}
