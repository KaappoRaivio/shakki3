#include <iostream>
#include <chrono>
#include "BitboardOperations.h"
#include "Board.h"
#include "Attacks.h"
#include "StringUtils.h"
#include "MyMath.h"
#include "BoardAnalysis.h"
#include "InBetween.h"
#include "ai/Search.h"
#include "ai/BoardEvaluator.h"
#include "ui/UI.h"
#include "Player.h"
#include "Runner.h"
#include "ui/HTTPUI.h"

int main () {
//    Board board = Board::fromFEN("8/3k4/3p4/pK1P1p2/P2P1P2/8/8/8 w - - 12 7");
//    Board board = Board::fromFEN("8/1K6/8/4q2P/8/8/5k2/8 b - - 3 2");



//    Board board = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board board = Board::fromFEN("rn1q1bnr/pp5p/2p1kp2/6p1/4p2N/8/PPPP1PPP/RNB1R1K1 b - - 0 1");

//    std::unique_ptr<UI> ui = std::make_unique<TtyUI>(board);

    const auto& aiPlayer = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});
//    const auto& aiPlayer = std::make_unique<AIPlayer>(13, std::chrono::seconds{10000000});

    std::unique_ptr<UI> ui = std::make_unique<HTTPUI>(aiPlayer.get());
    const auto& humanPlayer = std::make_unique<HumanPlayer>(ui.get());
    std::vector<Player*> players = {
            humanPlayer.get(),
            aiPlayer.get(),
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
