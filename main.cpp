#include <iostream>
#include "BitboardOperations.h"
#include "Board.h"
#include "SlidingPieceRays.h"
#include "KnightAttacks.h"
#include "KingAttacks.h"

int main () {
    Board board;
    std::cout << board << std::endl;


    SlidingPieceRays slidingPieceRays;
    KingAttacks attacks;

    Bitboard occupancy = 0x1e4000e640e04800;
    std::cout << occupancy << std::endl;
//    std::cout << BitboardOperations::bitboardToString(occupancy) << std::endl;
//
//    std::cout << BitboardOperations::bitboardToString(slidingPieceRays.getBishopBlockerMask(5, 5)) << std::endl;
    std::cout << attacks.getKingAttackAt(5, 5) << std::endl;
//    std::cout << attacks.getKingAttackAt(2, 1) << std::endl;
    std::cout << "done\n";

//    std::cout << SlidingPieceRays::rookSlides[NORTH][10] << std::endl;

//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
