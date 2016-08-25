#include "integration_tests.h"
#include "../core/board.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace test {

const std::string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void RunIntegrationTests() {
  std::vector<LegalMoveTest> tests;

  LegalMoveTest start_pos_test("starting position");
  start_pos_test.fen = STARTPOS;
  start_pos_test.legal_moves = { "a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3", "d2d4",
    "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4", "h2h3", "h2h4", "b1a3", "b1c3", "g1f3", "g1h3" };
  tests.push_back(start_pos_test);
  
  LegalMoveTest najdorf_test("Najdorf Sicilian");
  najdorf_test.fen = STARTPOS;
  najdorf_test.moves = { "e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4", "f3d4", "g8f6", "b1c3", "a7a6" };
  najdorf_test.legal_moves = { "a2a3", "a2a4", "b2b3", "b2b4", "e4e5", "f2f3", "f2f4", "g2g3", "g2g4",
    "h2h3", "h2h4", "a1b1", "c1d2", "c1e3", "c1f4", "c1g5", "c1h6", "d1d2", "d1d3", "d1e2", "d1f3",
    "d1g4", "d1h5", "e1d2", "e1e2", "f1e2", "f1d3", "f1c4", "f1b5", "f1a6", "h1g1" };
  tests.push_back(najdorf_test);

  LegalMoveTest empty_rook_test("empty rooks");
  empty_rook_test.fen = "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1";
  empty_rook_test.legal_moves = { "a8a7", "a8a6", "a8a5", "a8a4", "a8a3", "a8a2", "a8a1", "a8b8", "a8c8",
    "a8d8", "h8h7", "h8h6", "h8h5", "h8h4", "h8h3", "h8h2", "h8h1", "h8g8", "h8f8", "e8d8", "e8d7", "e8e7",
    "e8f7", "e8f8", "e8c8", "e8g8" };
  tests.push_back(empty_rook_test);
  
  LegalMoveTest knight_test("knights");
  knight_test.fen = "4k3/8/8/3N3N/8/8/8/4K2N w - - 0 1";
  knight_test.legal_moves = { "h1f2", "h1g3", "h5g7", "h5f6", "h5f4", "h5g3", "d5e7", "d5f6", "d5f4",
    "d5e3", "d5c3", "d5b4", "d5b6", "d5c7", "e1d1", "e1d2", "e1e2", "e1f2", "e1f1" };
  tests.push_back(knight_test);

  LegalMoveTest bishop_test("bishops");
  bishop_test.fen = "k7/4pp2/8/8/2b4b/8/PPPPPPPP/K7 b - - 0 1";
  bishop_test.legal_moves = { "a8a7", "a8b7", "a8b8", "e7e6", "e7e5", "f7f6", "f7f5", "c4b5", "c4a6", "c4d5", 
    "c4e6", "c4d3", "c4e2", "c4b3", "c4a2", "h4g5", "h4f6", "h3g3", "h4f2" };
  tests.push_back(bishop_test);
  
  LegalMoveTest queen_test("queens");
  queen_test.fen = "4k3/4p3/8/8/8/8/4P3/4K2Q w - - 0 1";
  queen_test.moves = { "h1e4", "e8d8" };
  queen_test.legal_moves = { "e4a8", "e4b7", "e4c6", "e4d5", "e4e5", "e4e6", "e4e7", "e4f5", "e4g6", "e4h7",
    "e4f4", "e4g4", "e4h4", "e4f3", "e4g2", "e4h1", "e4e3", "e4d3", "e4c2", "e4b1", "e4d4", "e4c4", "e4b4",
    "e4a4", "e2e3", "e1d2", "e1d1", "e1f2", "e1f1" };
  tests.push_back(queen_test);
  
  LegalMoveTest back_rank_mate_test("back rank mate");
  back_rank_mate_test.fen = "6k1/5ppp/8/8/8/8/5PPP/R5K1 w - - 0 1";
  back_rank_mate_test.moves = { "a1a8" };
  back_rank_mate_test.legal_moves = { };
  tests.push_back(back_rank_mate_test);
  
  LegalMoveTest stalemate_test("stalemate");
  stalemate_test.fen = "8/8/8/8/8/2k5/2p5/2K5 w - - 0 1";
  stalemate_test.legal_moves = { };
  tests.push_back(stalemate_test);
  
  LegalMoveTest pawn_promotion_test("pawn promotion");
  pawn_promotion_test.fen = "8/8/8/8/8/1k6/1p6/R5K1 b - - 0 1";
  pawn_promotion_test.legal_moves = { "b3b4", "b3c4", "b3c3", "b3c2", "b2a1q", "b2a1n", "b2a1r", "b2a1b",
    "b2b1q", "b2b1r", "b2b1b", "b2b1n" };
  tests.push_back(pawn_promotion_test);

  LegalMoveTest pawn_test("pawns");
  pawn_test.fen = "4k3/8/8/3ppp2/1p2P3/P1p2Pp1/2PP3P/4K3 w - - 0 1";
  pawn_test.legal_moves = { "a3a4", "a3b4", "d2c3", "d2d3", "d2d4", "e4d5", "e4f5", "f3f4", "h2g3", "h2h3",
    "h2h4", "e1d1", "e1e2", "e1f1" };
  tests.push_back(pawn_test);
  
  LegalMoveTest diagonal_pin_test("diagonal pins");
  diagonal_pin_test.fen = "4k3/3ppp2/8/7Q/B7/8/8/7K b - - 0 1";
  diagonal_pin_test.legal_moves = { "e8d8", "e8f8", "e7e6", "e7e5" };
  tests.push_back(diagonal_pin_test);
  
  LegalMoveTest early_check_test("early check");
  early_check_test.fen = STARTPOS;
  early_check_test.moves = { "e2e4", "d7d5", "g1f3", "f7f6", "b1c3", "a7a6", "f1b5" };
  early_check_test.legal_moves = { "a6b5", "c7c6", "b8c6", "b8d7", "c8d7", "d8d7", "e8f7" };
  tests.push_back(early_check_test);

  for (LegalMoveTest current_test : tests) {
    PerformTest(current_test);
  }
}

void PerformTest(LegalMoveTest test) {
  std::cout << "Testing " << test.test_name << std::endl;

  core::Board board;

  std::istringstream fen_parser(test.fen);
  std::string position, active_color, castle, en_passant, halfmove_clock, fullmove_number;
  fen_parser >> position;
  fen_parser >> active_color;
  fen_parser >> castle;
  fen_parser >> en_passant;
  fen_parser >> halfmove_clock;
  fen_parser >> fullmove_number;

  board.LoadFromFen(position, active_color, castle, en_passant, halfmove_clock, fullmove_number);

  for (std::string move : test.moves) {
    board.LoadMove(move);
  }

  board.Print();

  std::set<std::string> test_moves = board.GetLegalMoves();
  std::set<std::string> legal_but_omitted;
  std::set<std::string> illegal_but_listed;

  set_difference(test.legal_moves.begin(), test.legal_moves.end(),
    test_moves.begin(), test_moves.end(),
    inserter(legal_but_omitted, legal_but_omitted.end()));
  set_difference(test_moves.begin(), test_moves.end(),
    test.legal_moves.begin(), test.legal_moves.end(),
    inserter(illegal_but_listed, illegal_but_listed.end()));

  bool insufficient = legal_but_omitted.size() > 0;
  bool excess = illegal_but_listed.size() > 0;

  if (insufficient) {
    std::cerr << "The following moves are legal but were not listed: ";
    for (std::string move : legal_but_omitted) {
      std::cerr << move << " ";
    }
    std::cerr << std::endl;
  }
  if (excess) {
    std::cerr << "The following moves are illegal but were listed: ";
    for (std::string move : illegal_but_listed) {
      std::cerr << move << " ";
    }
    std::cerr << std::endl;
  }

  if (!insufficient && !excess) {
    std::cout << "PASS" << std::endl;
  }
}

}
