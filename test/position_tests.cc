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
    TestCastlingCancellation();
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
  TestHelper::AssertEqual((int)move.moving_piece, core::PAWN, "e2e4 involves the move of a pawn");
  TestHelper::AssertEqual((int)move.promoted_piece, (int)core::NULL_PIECE, "No promotion means null piece");
  TestHelper::AssertFalse(move.is_en_passant, "No en passant means en passant flag is false");
  TestHelper::AssertFalse(move.is_castle, "No castling means castling flag is false");
  TestHelper::AssertFalse(move.is_capture, "e2e4 is not a capture");
  
  TestHelper::AssertEqual(move.start_square.rank, 1, "rank of e2");
  TestHelper::AssertEqual(move.start_square.file, 4, "file of e2");
  TestHelper::AssertEqual(move.end_square.rank, 3, "rank of e4");
  TestHelper::AssertEqual(move.end_square.file, 4, "file of e4");
  
  Move illegal_move("h1h2", board.GetPosition());
  TestHelper::AssertTrue(illegal_move.is_illegal_collision, "Rh2 opening move is illegal");
  
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
  TestHelper::AssertEqual((int)position.GetActiveColor(), (int)core::WHITE, "FEN with w means White is next");
  
  board.LoadMove("h1f1");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "h1", core::EMPTY, "White rook expected to have left h1");
  TestHelper::AssertContentsAt(position, "f1", core::ROOK_W, "White rook expected on h1");
  TestHelper::AssertFalse(position.GetCastle().white_kingside, "h1f1 shut off O-O");
  TestHelper::AssertTrue(position.GetCastle().white_queenside, "h1f1 didn't shut off O-O-O");
  
  board.LoadMove("a8a7");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "a8", core::EMPTY, "Black rook expected to have left a8");
  TestHelper::AssertContentsAt(position, "a7", core::ROOK_B, "Black rook expected on a7");
  TestHelper::AssertTrue(position.GetCastle().black_kingside, "a8a7 didn't shut off O-O");
  TestHelper::AssertFalse(position.GetCastle().black_queenside, "a8a7 shut off O-O-O");
  
  Move move_e1c1("e1c1", position);
  TestHelper::AssertTrue(move_e1c1.is_castle, "e1c1 is castling");
  TestHelper::AssertFalse(move_e1c1.is_capture, "Castling is not a capture");
  
  board.LoadMove("e1c1");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e1", core::EMPTY, "Castling king left e1");
  TestHelper::AssertContentsAt(position, "d1", core::ROOK_W, "O-O-O rook on d1");
  TestHelper::AssertContentsAt(position, "c1", core::KING_W, "O-O-O king on c1");
  TestHelper::AssertContentsAt(position, "b1", core::EMPTY, "b1 empty after O-O-O");
  TestHelper::AssertContentsAt(position, "a1", core::EMPTY, "a1 empty after O-O-O");
  TestHelper::AssertFalse(position.GetCastle().white_kingside, "O-O-O shut off white O-O");
  TestHelper::AssertFalse(position.GetCastle().white_queenside, "O-O-O shut off white O-O-O");
  
  Move move_e8g8("e8g8", position);
  TestHelper::AssertTrue(move_e8g8.is_castle, "e8g8 is castling");
  TestHelper::AssertEqual((int)move_e8g8.moving_piece, (int)core::KING, "The king moves in a castle");
  
  Move move_e8f8(Square("e8"), Square("f8"), position);
  TestHelper::AssertFalse(move_e8f8.is_castle, "e8f8 is not castling");
  
  board.LoadMove("e8g8");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e8", core::EMPTY, "Castling king left e8");
  TestHelper::AssertContentsAt(position, "f8", core::ROOK_B, "O-O rook on f8");
  TestHelper::AssertContentsAt(position, "g8", core::KING_B, "O-O king on g8");
  TestHelper::AssertContentsAt(position, "h8", core::EMPTY, "h8 empty after O-O");
  TestHelper::AssertFalse(position.GetCastle().black_kingside, "O-O shut off black O-O");
  TestHelper::AssertFalse(position.GetCastle().black_queenside, "O-O-O shut off black O-O-O");
}

// White gets rook captured, Black moves king.
void TestCastlingCancellation() {
  Board board = TestHelper::CreateBoardFromFen("r3k2r/pb1p1p1p/4p3/8/8/6P1/P2BPP1P/R3K2R b KQkq - 0 1");
  
  Position position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "d2", core::BISHOP_W, "White bishop expected on d2");
  TestHelper::AssertContentsAt(position, "b7", core::BISHOP_B, "Black bishop expected on b7");
  TestHelper::AssertEqual((int)position.GetActiveColor(), (int)core::BLACK, "FEN with b means Black is next");
  
  board.LoadMove("b7h1");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "b7", core::EMPTY, "b7h1 left b7 empty");
  TestHelper::AssertContentsAt(position, "h1", core::BISHOP_B, "b7h1 captured h1 and left Black bishop");
  TestHelper::AssertFalse(position.GetCastle().white_kingside, "Kingside rook capture turns off castling flag");
  TestHelper::AssertTrue(position.GetCastle().white_queenside, "Kingside rook capture doesn't affect queenside");
  
  board.LoadMove("d2h6");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "d2", core::EMPTY, "d2h6 left d2 empty");
  TestHelper::AssertContentsAt(position, "h6", core::BISHOP_W, "d2h6 puts White bishop on h6");
  TestHelper::AssertTrue(position.GetCastle().black_queenside, "Black O-O-O unaffected");
  TestHelper::AssertTrue(position.GetCastle().black_kingside, "Black O-O is illegal but still possible later");
  
  board.LoadMove("e8e7");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "e8", core::EMPTY, "e8e7 left e8 empty");
  TestHelper::AssertContentsAt(position, "e7", core::KING_B, "e8e7 puts Black king on e7");
  TestHelper::AssertFalse(position.GetCastle().black_queenside, "King move blocks O-O");
  TestHelper::AssertFalse(position.GetCastle().black_kingside, "King move blocks O-O-O");
}

// Early en passant sequences starting from STARTPOS
void TestEnPassant() {
  Board board = TestHelper::CreateBoardFromFen(STARTPOS);
  
  board.LoadMove("d2d4");
  Position position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "d3", "d2d4 leaves d3 en passant");
  
  Move move_f7f5("f7f5", position);
  TestHelper::AssertFalse(move_f7f5.is_en_passant, "f7f5 is not en passant");
  
  board.LoadMove("f7f5");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "f6", "f7f5 leaves f6 en passant");
  
  board.LoadMove("d4d5");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "d4d5 cancels en passant");
  
  board.LoadMove("c7c5");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "c6", "c7c5 leaves c6 en passant");
  
  Move move_d5c6("d5c6", position);
  TestHelper::AssertTrue(move_d5c6.is_en_passant, "d5c6 is en passant");
  
  board.LoadMove("d5c6");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "d5", core::EMPTY, "d5c6 leaves d5 empty");
  TestHelper::AssertContentsAt(position, "c6", core::PAWN_W, "d5c6 puts White pawn on c6");
  TestHelper::AssertContentsAt(position, "c5", core::EMPTY, "d5c6 en passant empties c5");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "En passant capture leaves no en passant");
  
  board.LoadMove("f5f4");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "f5f4 leaves no en passant");
  
  board.LoadMove("e2e4");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "e3", "e2e4 leaves e3 en passant");
  
  board.LoadMove("b8c6");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "Inaction expires en passant");
  
  board.LoadMove("g2g4");
  position = board.GetPosition();
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "g3", "g2g4 leaves g3 en passant");
  
  Move move_f4g3(Square(3, 5), Square(2, 6), position);
  TestHelper::AssertTrue(move_f4g3.is_en_passant, "f4g3 is en passant");
  
  board.LoadMove("f4g3");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "f4", core::EMPTY, "f4g3 leaves f4 empty");
  TestHelper::AssertContentsAt(position, "g3", core::PAWN_B, "f4g3 puts Black pawn on g3");
  TestHelper::AssertContentsAt(position, "g4", core::EMPTY, "f4g3 en passant empties g4");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "En passant capture leaves no en passant");
}

// Contrived sequence with four promotions
void TestPromotion() {
  Board board = TestHelper::CreateBoardFromFen("7n/P5P1/8/8/6K1/8/1pkp4/8 w - - 0 1");
  
  Position position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "h8", core::KNIGHT_B, "Black knight expected on h8");
  TestHelper::AssertFalse(position.GetCastle().white_kingside, "No castling in this endgame position");
  
  Move move_g7h8q(Square("g7"), Square("h8"), position, core::QUEEN);
  TestHelper::AssertTrue(move_g7h8q.is_capture, "g7h8q is a pawn capture");
  TestHelper::AssertEqual(move_g7h8q.ToString(), "g7h8q", "g7h8q to string");
  TestHelper::AssertEqual(move_g7h8q.start_square.ToString(), "g7", "g7h8q's start square is g7");
  TestHelper::AssertEqual(move_g7h8q.end_square.ToString(), "h8", "g7h8q's end square is h8");
  TestHelper::AssertEqual((int)move_g7h8q.promoted_piece, (int)core::QUEEN, "g7h8q's promoted piece is a q");
  
  board.LoadMove("g7h8q");
  position = board.GetPosition();
  TestHelper::AssertEqual((int)position.GetActiveColor(), (int)core::BLACK, "Black's turn after White's promotion move");
  TestHelper::AssertContentsAt(position, "g7", core::EMPTY, "g7h8q leaves g7 empty");
  TestHelper::AssertContentsAt(position, "h8", core::QUEEN_W, "g7h8q leaves White queen on h8");
  
  Move move_b2b1n("b2b1n", position);
  TestHelper::AssertEqual(move_b2b1n.ToString(), "b2b1n", "b2b1n to string");
  TestHelper::AssertEqual((int)move_b2b1n.moving_piece, (int)core::PAWN, "b2b1n is considered a pawn move");
  TestHelper::AssertEqual((int)move_b2b1n.promoted_piece, (int)core::KNIGHT, "b2b1n's promoted piece is an n");
  TestHelper::AssertFalse(move_b2b1n.is_capture, "b2b1n is not a capture");
  TestHelper::AssertFalse(move_b2b1n.is_en_passant, "b2b1n is not en passant");
  
  board.LoadMove("b2b1n");
  position = board.GetPosition();
  TestHelper::AssertEqual((int)position.GetActiveColor(), (int)core::WHITE, "White's turn after Black's promotion move");
  TestHelper::AssertContentsAt(position, "b2", core::EMPTY, "b2b1n leaves b2 empty");
  TestHelper::AssertContentsAt(position, "b1", core::KNIGHT_B, "b2b1n leaves Black knight on b1");
  
  board.LoadMove("a7a8r");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "a7", core::EMPTY, "a7a8r leaves a7 empty");
  TestHelper::AssertContentsAt(position, "a8", core::ROOK_W, "a7a8r leaves White rook on a8");
  TestHelper::AssertEqual(position.GetEnPassant().ToString(), "-", "Promotion leaves no en passant behind");
  
  board.LoadMove("d2d1b");
  position = board.GetPosition();
  TestHelper::AssertContentsAt(position, "d2", core::EMPTY, "d2d1b leaves d2 empty");
  TestHelper::AssertContentsAt(position, "d1", core::BISHOP_B, "d2d1b leaves Black bishop on d1");
  TestHelper::AssertFalse(position.GetCastle().black_queenside, "Queenside castling rights don't return");
}

}
