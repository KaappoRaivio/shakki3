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
#include "src/ui/HTTPUI.h"
#include "tests/TestHelpers.h"

int main () {
//    Board board = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

//    Board board = Board::fromFEN("r2qk2r/2pbbpp1/3p1nnp/p2Pp3/4P3/pB2BNNP/1PP2PP1/2RQ1RK1 w kq - 0 15");
    Board board = Board::fromFEN("1r3bk1/2p2pp1/3p4/1B1P1P1n/p2Q1p1p/q1PN1P1P/6P1/3R1K2 w - - 0 1");

    //    Board board = Board::fromFEN("rn1q1bnr/pp5p/2p1kp2/6p1/4p2N/8/PPPP1PPP/RNB1R1K1 b - - 0 1");
//    std::cout << BoardEvaluator::evaluateSimple(board, 1, 1) << std::endl;
//    return 0;





//    std::unique_ptr<UI> ui = std::make_unique<TtyUI>(board);

    const auto& aiPlayer = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});
//    const auto& aiPlayer = std::make_unique<AIPlayer>(13, std::chrono::seconds{10000000});

    std::unique_ptr<UI> ui = std::make_unique<HTTPUI>(aiPlayer.get());
    const auto& humanPlayer = std::make_unique<HumanPlayer>(ui.get());
    std::vector<Player*> players = {
            aiPlayer.get(),
            humanPlayer.get(),
//            aiPlayer2.get()
    };

    Runner runner {players, board, ui.get()};
    runner.play();

//    std::cout << board << std::endl;
//    std::cout << board.toFEN() << std::endl;
//    Search search{};
////
//
//    std::vector<Move> PV;
//    auto startTime = std::chrono::system_clock::now();
//    std::cout << search.getBestMove(board, 30) << std::endl;
////    std::cout << search.negamaxSearch(board, 3, PV) << std::endl;
////    std::cout << BoardEvaluator::evaluateSimple(board, 1) << std::endl;
//    auto endTime = std::chrono::system_clock::now();
//    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms!" << std::endl;

}
