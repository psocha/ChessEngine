#include "legal_move_tests.h"
#include "test_main.h"
#include "../core/board.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace test {

bool RunLegalMoveTests() {
  std::vector<LegalMoveTest> tests;

  LegalMoveTest start_pos_test("starting position");
  start_pos_test.fen = STARTPOS;
  start_pos_test.legal_moves = { "a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3", "d2d4",
    "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4", "h2h3", "h2h4", "b1a3", "b1c3", "g1f3", "g1h3" };
  tests.push_back(start_pos_test);

  LegalMoveTest najdorf_test("Najdorf Sicilian");
  najdorf_test.fen = STARTPOS;
  najdorf_test.moves = { "e2e4", "c7c5", "g1f3", "d7d6", "d2d4", "c5d4", "f3d4", "g8f6", "b1c3", "a7a6" };
  najdorf_test.moves = { "e2e4" };
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
  
  LegalMoveTest orthogonal_pin_test("orthogonal pin");
  orthogonal_pin_test.fen = "k7/8/8/8/8/8/8/KQ5r w - - 0 1";
  orthogonal_pin_test.legal_moves = { "a1a2", "a1b2", "b1c1", "b1d1", "b1e1", "b1f1", "b1g1", "b1h1" };
  tests.push_back(orthogonal_pin_test);
  
  LegalMoveTest early_check_test("early check");
  early_check_test.fen = STARTPOS;
  early_check_test.moves = { "e2e4", "d7d5", "g1f3", "f7f6", "b1c3", "a7a6", "f1b5" };
  early_check_test.legal_moves = { "a6b5", "c7c6", "b8c6", "b8d7", "c8d7", "d8d7", "e8f7" };
  tests.push_back(early_check_test);
  
  LegalMoveTest endgame_check_test("endgame check");
  endgame_check_test.fen = "7b/8/b3k3/8/8/8/4R3/4K3 b - - 0 1";
  endgame_check_test.legal_moves = { "a6e2", "h8e5", "e6d5", "e6d6", "e6d7", "e6f5", "e6f6", "e6f7" };
  tests.push_back(endgame_check_test);
  
  LegalMoveTest defended_checker_test("defended checker");
  defended_checker_test.fen = "5q2/8/6k1/8/5PP1/6K1/8/2B5 w - - 0 1";
  defended_checker_test.moves = { "f4f5" };
  defended_checker_test.legal_moves = { "g6f6", "g6f7", "g6g7", "g6h7", "f8f5" };
  tests.push_back(defended_checker_test);
  
  LegalMoveTest no_castle_out_of_check_test("no castle out of check");
  no_castle_out_of_check_test.fen = "r3k2r/p6p/8/8/8/8/8/4R1K1 b kq - 0 1";
  no_castle_out_of_check_test.legal_moves = { "e8d8", "e8d7", "e8f8", "e8f7" };
  tests.push_back(no_castle_out_of_check_test);
  
  LegalMoveTest no_castle_through_check_test("no castle through check");
  no_castle_through_check_test.fen = "4k3/8/8/8/8/2n4b/P6P/R3K2R w KQ - 0 1";
  no_castle_through_check_test.legal_moves = { "a1b1", "a1c1", "a1d1", "a2a3", "a2a4", "e1d2", "e1f2",
    "h1g1", "h1f1" };
  tests.push_back(no_castle_through_check_test);
  
  LegalMoveTest no_castle_into_check_test("no castle into check");
  no_castle_into_check_test.fen = "r3k2r/p6p/1P6/8/8/8/5PPP/R4RK1 w kq - 0 1";
  no_castle_into_check_test.moves = { "b6b7" };
  no_castle_into_check_test.legal_moves = { "a7a6", "a7a5", "a8b8", "a8c8", "a8d8", "e8d8", "e8d7",
    "e8e7", "e8f8", "e8f7", "e8g8", "h8g8", "h8f8", "h7h6", "h7h5" };
  tests.push_back(no_castle_into_check_test);
  
  LegalMoveTest no_castle_without_flag_test("no castle without flag");
  no_castle_without_flag_test.fen = "5rk1/5ppp/6q1/3pp3/4P3/5P2/P5PP/R3K2R w Q - 0 1";
  no_castle_without_flag_test.legal_moves = { "a1b1", "a1c1", "a1d1", "a2a3", "a2a4", "e4d5", "f3f4",
    "g2g3", "g2g4", "h2h3", "h2h4", "e1c1", "e1d1", "e1d2", "e1e2", "e1f2", "e1f1", "h1g1", "h1f1" };
  tests.push_back(no_castle_without_flag_test);
  
  LegalMoveTest no_castle_if_blocked_test("no castle if blocked");
  no_castle_if_blocked_test.fen = "r3kb1r/1pqbn1p1/pnp1pp1p/3p4/2PP4/1QN1PN2/PP1BBPPP/3RR1K1 b kq - 0 1";
  no_castle_if_blocked_test.legal_moves = { "a6a5", "a8a7", "a8b8", "a8c8", "a8d8", "b6c4", "b6a4", "b6c8",
    "c7b8", "c7c8", "c7d8", "c7d6", "c7e5", "c7f4", "c7h3", "c7g2", "d7c8", "d5c4", "e8d8", "e8c8", "e7c8",
    "e7g8", "e7g6", "e7f5", "e6e5", "f6f5", "g7g6", "g7g5", "h8g8", "h8h7", "h6h5" };
  tests.push_back(no_castle_if_blocked_test);
  
  LegalMoveTest no_castle_if_king_moves_test("no castle if king moves");
  no_castle_if_king_moves_test.fen = "5rk1/5pbp/5np1/8/8/8/P6P/R3K2R w KQ - 0 1";
  no_castle_if_king_moves_test.moves = { "e1e2", "g8h8", "e2e1", "h8g8" };
  no_castle_if_king_moves_test.legal_moves = { "a2a3", "a2a4", "h2h3", "h2h4", "a1b1", "a1c1", "a1d1",
    "h1g1", "h1f1", "e1d1", "e1d2", "e1e2", "e1f2", "e1f1" };
  tests.push_back(no_castle_if_king_moves_test);
  
  LegalMoveTest no_castle_if_rook_moves_test("no castle if rook moves");
  no_castle_if_rook_moves_test.fen = "r3k2r/p6p/8/8/8/P7/1PPP4/2KR3R b kq - 0 1";
  no_castle_if_rook_moves_test.moves = { "a8d8", "h1h7", "d8a8", "h7h1" };
  no_castle_if_rook_moves_test.legal_moves = { "a8b8", "a8c8", "a8d8", "a7a6", "a7a5", "e8d8", "e8d7",
    "e8e7", "e8f7", "e8f8", "e8g8", "h8g8", "h8f8", "h8h7", "h8h6", "h8h5", "h8h4", "h8h3", "h8h2",
	"h8h1" };
  tests.push_back(no_castle_if_rook_moves_test);
  
  LegalMoveTest en_passant_test("en passant");
  en_passant_test.fen = "3nkn2/3ppp2/8/8/8/8/3PPP2/3NKN2 w - - 0 1";
  en_passant_test.moves = { "e2e4", "d7d5", "e4e5", "f7f5" };
  en_passant_test.legal_moves = { "d2d3", "d2d4", "e5e6", "e5f6", "f2f3", "f2f4", "d1b2", "d1c3",
    "d1e3", "f1e3", "f1g3", "f1h2", "e1e2" };
  tests.push_back(en_passant_test);
  
  LegalMoveTest en_passant_fen_test("en passant fen square");
  en_passant_fen_test.fen = "8/8/5k2/8/2B2pPb/8/8/6K1 b - g3 0 1";
  en_passant_fen_test.legal_moves = { "f6e5", "f6e7", "f6g7", "f6g6", "f6g5", "h4g5", "h4g3", "h4f2",
    "h4e1", "f4f3", "f4g3" };
  tests.push_back(en_passant_fen_test);
  
  LegalMoveTest en_passant_expiry_test("en passant expiry");
  en_passant_expiry_test.fen = "k7/2p5/1p6/pP6/2PP4/8/K7/8 w - a6 0 1";
  en_passant_expiry_test.moves = { "a2b3", "c7c5" };
  en_passant_expiry_test.legal_moves = { "b3a4", "b3a3", "b3a2", "b3b2", "b3c3", "b3c2", "d4c5", "d4d5",
    "b5c6" };
  tests.push_back(en_passant_expiry_test);
  
  LegalMoveTest perform_castle_kingside_test("perform castle kingside");
  perform_castle_kingside_test.fen = "4k2r/6pp/8/8/8/8/6PP/R5K1 b k - 0 1";
  perform_castle_kingside_test.moves = { "e8g8", "a1a7" };
  perform_castle_kingside_test.legal_moves = { "f8e8", "f8d8", "f8c8", "f8b8", "f8a8", "f8f7", "f8f6",
    "f8f5", "f8f4", "f8f3", "f8f2", "f8f1", "g7g6", "g7g5", "h7h6", "h7h5", "g8h8" };
  tests.push_back(perform_castle_kingside_test);
  
  LegalMoveTest perform_castle_queenside_test("perform castle queenside");
  perform_castle_queenside_test.fen = "8/8/8/8/8/2P3k1/3P3p/R3K3 w Q - 0 1";
  perform_castle_queenside_test.moves = { "e1c1", "g3g2" };
  perform_castle_queenside_test.legal_moves = { "c1b1", "c1b2", "c1c2", "c3c4", "d2d3", "d2d4", "d1e1",
    "d1f1", "d1g1", "d1h1" };
  tests.push_back(perform_castle_queenside_test);
  
  LegalMoveTest perform_promotion_test("perform promotion");
  perform_promotion_test.fen = "8/2PP4/1k1K4/8/8/8/8/8 w - - 0 1";
  perform_promotion_test.moves = { "c7c8n", "b6b5", "d7d8b", "b5c4" };
  perform_promotion_test.legal_moves = { "c8a7", "c8b6", "c8e7", "d8c7", "d8b6", "d8a5", "d8e7", "d8f6",
    "d8g5", "d8h4", "d6c6", "d6c7", "d6d7", "d6e7", "d6e6", "d6e5" };  
  tests.push_back(perform_promotion_test);
  
  LegalMoveTest perform_en_passant_test("perform en passant");
  perform_en_passant_test.fen = "nk6/8/8/3N4/p7/8/1P6/1K6 w - - 0 1";
  perform_en_passant_test.moves = { "b2b4", "a4b3" };
  perform_en_passant_test.legal_moves = { "b1a1", "b1b2", "b1c1", "d5b4", "d5b6", "d5c7", "d5e7", "d5f6",
    "d5f4", "d5e3", "d5c3" };
  tests.push_back(perform_en_passant_test);
  
  LegalMoveTest deep_blue_test("deep blue");
  deep_blue_test.fen = "r1k4r/p2nb1p1/2b4p/1p1n1p2/2PP4/3Q1NB1/1P3PPP/R5K1 b - - 0 1";
  deep_blue_test.legal_moves = { "a7a6", "a7a5", "b5c4", "b5b4", "f5f4", "g7g6", "g7g5", "h6h5", "a8b8",
    "c8b8", "c8b7", "c8c7", "c8d8", "c6b7", "d7b8", "d7b6", "d7c5", "d7e5", "d7f6", "d7f8", "d5c7", "d5b6",
    "d5b4", "d5c3", "d5e3", "d5f4", "d5f6", "e7d8", "e7f8", "e7f6", "e7g5", "e7h4", "e7d6", "e7c5", "e7b4",
    "e7a3" };
  tests.push_back(deep_blue_test);
  
  LegalMoveTest byrne_fischer_test("byrne fischer");
  byrne_fischer_test.fen = "r2q1rk1/pp2ppbp/1np2np1/2Q3B1/3PP1b1/2N2N2/PP3PPP/3RKB1R b K - 0 1";
  byrne_fischer_test.legal_moves = { "a7a6", "a7a5", "e7e6", "e7e5", "h7h6", "h7h5", "a8b8", "a8c8", "b6c8",
    "b6d7", "b6d5", "b6c4", "b6a4", "d8c8", "d8b8", "d8c7", "d8d7", "d8d6", "d8d5", "d8d4", "d8e8", "f8e8",
    "g8h8", "g7h8", "g7h6", "f6e8", "f6h5", "f6e4", "f6d5", "f6d7", "g4f3", "g4h3", "g4h5", "g4f5", "g4e6",
    "g4d7", "g4c8" };
  tests.push_back(byrne_fischer_test);

  LegalMoveTestResult aggregate;
  int positions_passed = 0, positions_failed = 0;
  for (LegalMoveTest current_test : tests) {
    LegalMoveTestResult result = PerformTest(current_test);

    aggregate.correct += result.correct;
    aggregate.omissions += result.omissions;
    aggregate.false_positives += result.false_positives;

    if (result.omissions == 0 && result.false_positives == 0) {
      positions_passed++;
    } else {
      positions_failed++;
    }
  }
  
  std::cout << "--------------------------------------------------------------------\n";
  std::cout << "LEGAL MOVE TEST SUMMARY\n";
  std::cout << positions_passed << " of " << positions_passed + positions_failed << " positions passed\n";
  std::cout << aggregate.correct << " moves correct\n";
  std::cout << aggregate.omissions << " moves omitted\n";
  std::cout << aggregate.false_positives << " illegal moves\n";
  
  return (aggregate.omissions == 0 && aggregate.false_positives == 0);
}

LegalMoveTestResult PerformTest(LegalMoveTest test) {
  std::cout << "Testing " << test.test_name << std::endl;
  LegalMoveTestResult result;

  core::Board board = TestHelper::CreateBoardFromFen(test.fen);

  for (std::string move : test.moves) {
    board.LoadMove(move);
  }

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
  
  result.correct = test.legal_moves.size() - legal_but_omitted.size();
  result.omissions = legal_but_omitted.size();
  result.false_positives = illegal_but_listed.size();
  
  if (insufficient || excess) {
    board.Print();
  }

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
  
  return result;
}

}
