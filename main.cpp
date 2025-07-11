#include <iostream>
#include <chrono>
#include "src/BitboardOperations.h"
#include "src/Board.h"
#include "src/Attacks.h"
#include "src/StringUtils.h"
#include "src/MyMath.h"
#include "src/BoardAnalysis.h"
#include "src/InBetween.h"
#include "src/ai/Search.h"
#include "src/ai/BoardEvaluator.h"
#include "src/ui/UI.h"
#include "src/Player.h"
#include "src/Runner.h"
//#include "src/ui/HTTPUI.h"
#include "tests/TestHelpers.h"

int main () {
    Board board = Board::fromFEN("1r3bk1/2p2pp1/3p4/1B1P1P1n/p2Q1p1p/q1PN1P1P/6P1/3R1K2 w - - 0 1");
//    Board board = Board::fromFEN("5bk1/5pp1/3p4/3P4/5p1p/2Pn1P1P/qr4P1/5RK1 w - - 0 13");
    board.executeSequenceOfMoves({"d4a4"});



    std::unique_ptr<UI> ui = std::make_unique<TtyUI>(board);

    const auto& aiPlayer = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});

    const auto& humanPlayer = std::make_unique<HumanPlayer>(ui.get());
    std::vector<Player*> players = {
            humanPlayer.get(),
            aiPlayer.get(),
    };

    Runner runner {players, board, ui.get()};
    runner.play();
}
