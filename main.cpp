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
#include "UI.h"
#include "Player.h"
#include "Runner.h"

int main () {

//    Board board = Board::fromFEN("8/7p/4kp2/8/2P1r3/8/5K1R/8 w - - 0 1");
//    Board board = Board::fromFEN("5k2/8/8/3Q4/8/7K/8/8 w - - 0 1");
    Board board = Board::fromFEN("8/2k5/8/8/3R4/8/5K2/8 w - - 0 1");
//    Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
//    Board board = Board::fromFEN("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1");
//    board.executeSequenceOfMoves({"a1b1", "a7b7", "b1c1", "b7c7"});
//    std::vector<Move> moves = {{board, e2, a6}};
//    std::cout << MyUtils::toString(moves) << std::endl;


    std::unique_ptr<UI> ui = std::make_unique<TtyUI>(board);

    const auto& aiPlayer = std::make_unique<AIPlayer>(7);
    const auto& humanPlayer = std::make_unique<HumanPlayer>(ui.get());
    std::vector<Player*> players = {
            aiPlayer.get(),
            humanPlayer.get()
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
