#include "position_tests.h"
#include "test_main.h"
#include "../core/board.h"
#include "../core/move.h"
#include "../core/square.h"

#include <iostream>
using core::Board;
using core::Move;
using core::Position;
using core::Square;

namespace test {

bool RunAllPositionTests() {
  try {
    TestSimpleSquaresAndMoves();
    TestCastlingPerformance();
    TestEnPassant();
    TestPromotion();
  } catch (FailingTestException fail) {
    std::cout << fail.GetMessage() << std::endl;
    return false;
  }
  
  return true;
}

// STARTPOS: e4 Nc6 e5 Nxe5
void TestSimpleSquaresAndMoves() {
  Board board = TestHelper::CreateBoardFromFen(STARTPOS);
  
  Move move = Move("e2e4", board.GetPosition());
  
  TestHelper::AssertEqual(move.ToString(), "e2e4", "move ToString()");
  TestHelper::AssertEqual(move.start_square.ToString(), "e2", "start square ToString()");
  TestHelper::AssertEqual(move.end_square.ToString(), "e4", "end square ToString()");
  
  TestHelper::AssertEqual(move.start_square.rank, 1, "rank of e2");
  TestHelper::AssertEqual(move.start_square.file, 4, "file of e2");
  TestHelper::AssertEqual(move.end_square.rank, 3, "rank of e4");
  TestHelper::AssertEqual(move.end_square.file, 4, "file of e4");
  
  board.LoadMove("e2e4");
  Position position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e2", core::EMPTY, "e2e4 leaves e2 vacant");
  TestHelper::AssertContentsAt(position, "e4", core::PAWN_W, "e2e4 puts pawn on e4");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "e3", "e2e4 leaves e3 en passant");
  
  board.LoadMove("b8c6");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "b8", core::EMPTY, "b8c6 leaves b8 vacant");
  TestHelper::AssertContentsAt(position, "c6", core::KNIGHT_B, "b8c6 puts knight on c6");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "En passant expiry");
  
  board.LoadMove("e4e5");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e4", core::EMPTY, "e4e5 leaves e4 vacant");
  TestHelper::AssertContentsAt(position, "e5", core::PAWN_W, "e4e5 puts pawn on e5");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "No en passant after single pawn move");
  
  board.LoadMove("c6e5");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "c6", core::EMPTY, "c6e5 leaves c6 vacant");
  TestHelper::AssertContentsAt(position, "e5", core::KNIGHT_B, "c6e5 captures e5");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "No en passant after knight move");
  
  TestHelper::AssertTrue(position.GetCastle().white_kingside, "white O-O still allowed");
  TestHelper::AssertTrue(position.GetCastle().black_kingside, "black O-O still allowed");
  TestHelper::AssertTrue(position.GetCastle().white_queenside, "white O-O-O still allowed");
  TestHelper::AssertTrue(position.GetCastle().black_queenside, "black O-O-O still allowed");
}

// Blank first ranks with kings and rooks all starting in castling position.
void TestCastlingPerformance() {
  Board board = TestHelper::CreateBoardFromFen("r3k2r/5ppp/p7/8/8/8/PPP4P/R3K2R w KQkq - 0 1");
  Position position = board.GetPosition();
  
  TestHelper::AssertContentsAt(position, "a6", core::PAWN_B, "Black pawn expected on a6");
  TestHelper::AssertContentsAt(position, "c2", core::PAWN_W, "White pawn expected on c2");
  
  board.LoadMove("h1f1");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "h1", core::EMPTY, "White rook expected to have left h1");
  TestHelper::AssertContentsAt(position, "f1", core::ROOK_W, "White rook expected on h1");
  TestHelper::AssertFalse(position.GetCastle().white_kingside, "h1f1 shut off O-O");
  TestHelper::AssertTrue(position.GetCastle().white_queenside, "h1f1 didn't shut off O-O-O");
  
  board.LoadMove("a8a7");
  position = board.GetPosition();
  position.Print();
  TestHelper::AssertContentsAt(position, "a8", core::EMPTY, "Black rook expected to have left a8");
  TestHelper::AssertContentsAt(position, "a7", core::ROOK_B, "Black rook expected on a7");
  TestHelper::AssertTrue(position.GetCastle().black_kingside, "a8a7 didn't shut off O-O");
  TestHelper::AssertFalse(position.GetCastle().black_queenside, "a8a7 shut off O-O-O");
  
  board.LoadMove("e1c1");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e1", core::EMPTY, "Castling king left e1");
  TestHelper::AssertContentsAt(position, "d1", core::ROOK_W, "O-O-O rook on d1");
  TestHelper::AssertContentsAt(position, "c1", core::KING_W, "O-O-O king on c1");
  TestHelper::AssertContentsAt(position, "b1", core::EMPTY, "b1 empty after O-O-O");
  TestHelper::AssertContentsAt(position, "a1", core::EMPTY, "a1 empty after O-O-O");
  TestHelper::AssertFalse(position.GetCastle().white_kingside, "O-O-O shut off white O-O");
  TestHelper::AssertFalse(position.GetCastle().white_queenside, "O-O-O shut off white O-O-O");
  
  board.LoadMove("e8g8");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e8", core::EMPTY, "Castling king left e8");
  TestHelper::AssertContentsAt(position, "f8", core::ROOK_B, "O-O rook on f8");
  TestHelper::AssertContentsAt(position, "g8", core::KING_B, "O-O king on g8");
  TestHelper::AssertContentsAt(position, "h8", core::EMPTY, "h8 empty after O-O");
  TestHelper::AssertFalse(position.GetCastle().black_kingside, "O-O shut off black O-O");
  TestHelper::AssertFalse(position.GetCastle().black_queenside, "O-O-O shut off black O-O-O");
}

void TestCastlingCancellation() {
  
}

void TestEnPassant() {
  
}

void TestPromotion() {
  
}

}
