#include "ai/random_ai.h"
#include "ai/material_ai.h"
#include "core/board.h"
#include "test/test_main.h"

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

void PrintHelp() {
  std::cout << std::endl;
  std::cout << "ChessEngine Paramters           | Output" << std::endl;
  std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << "./ChessEngine.exe RandomAI      | An AI that plays random legal moves" << std::endl;
  std::cout << "./ChessEngine.exe MaterialAI    | An AI that prioritizes material" << std::endl;
  std::cout << "./ChessEngine.exe --runtests    | Run the engine's set of unit tests and exit (not a real chess engine)" << std::endl;
  std::cout << "./ChessEngine.exe --help        | Display this help dialog" << std::endl;
  std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
}

int main(int argc, char* argv[]) {
  string line;
  cout.setf(ios::unitbuf);

  core::Board *board = new core::Board();
  ai::ChessAI *chess_ai = NULL;
  
  if (argc > 1 && string(argv[1]) == "RandomAI") {
    chess_ai = new ai::RandomAI();
  } else if (argc > 1 && string(argv[1]) == "MaterialAI") {
    chess_ai = new ai::MaterialAI();
  } else if (argc > 1 && string(argv[1]) == "--runtests") {
    test::RunAllTests();
    return 0;
  } else if (argc > 1 && string(argv[1]) == "--help") {
    PrintHelp();
    return 0;
  } else {
    std::cout << "Missing command-line flags. See --help for options." << std::endl;
    return 0;
  }

  while (getline(cin, line)) {
    if (line == "uci") {
      cout << "id name random" << endl;
      cout << "id author psocha" << endl;
    }
    else if (line == "isready") {
      cout << "readyok" << endl;
    }
    else if (line.substr(0, 9) == "position ") {
      istringstream position_line(line);
      string current_word;
      position_line >> current_word;
      position_line >> current_word;
      if (current_word == "startpos") {
        board->LoadStartPos();
      }
      else {
        string position, active_color, castle, en_passant, halfmove_clock, fullmove_number;
        position_line >> position;
        position_line >> active_color;
        position_line >> castle;
        position_line >> en_passant;
        position_line >> halfmove_clock;
        position_line >> fullmove_number;

        board->LoadFromFen(position, active_color, castle, en_passant, halfmove_clock, fullmove_number);
      }

      while (position_line >> current_word) {
        if (current_word == "moves") continue;
        board->LoadMove(current_word);
      }
    }
    else if (line.substr(0, 3) == "go ") {
      core::Position current_position = board->GetPosition();
      string best_move = chess_ai->BestMove(current_position);
      cout << "bestmove " << best_move << endl;
    }
    else if (line == "quit") {
      break;
    }
  }

  delete board;
  delete chess_ai;

  return 0;
}
