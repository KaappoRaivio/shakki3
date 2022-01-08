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

int main () {

//    Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Board board = Board::fromFEN("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1");
    board.executeSequenceOfMoves({"a1b1", "a7b7", "b1c1", "b7c7"});
    std::cout << board << std::endl;
    std::cout << board.toFEN() << std::endl;
    Search search{};

    auto startTime = std::chrono::system_clock::now();
    std::cout << search.getBestMove(board, 20) << std::endl;
    auto endTime = std::chrono::system_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms!" << std::endl;

}
