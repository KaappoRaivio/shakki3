//
// Created by kaappo on 22.11.2021.
//

#include <stdexcept>
#include <iostream>
#include "SlidingPieceRays.h"

using namespace BitboardOperations;
using namespace BitboardOperations::Directions;

void SlidingPieceRays::populateRookSlides () {
    for (int direction = NORTH; direction <= WEST; direction++) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Bitboard start = 1ULL << (8 * y + x);
                Bitboard originalStart = start;
                Bitboard other = start;
                while (true) {
                    if (direction == NORTH) {
                        if (start & SquareMasks::eighthRank) {
                            break;
                        }
                    } else if (direction == SOUTH) {
                        if (start & SquareMasks::firstRank) {
                            break;
                        }
                    } else if (direction == EAST) {
                        if (start & SquareMasks::hFile) {
                            break;
                        }
                    } else if (direction == WEST) {
                        if (start & SquareMasks::aFile) {
                            break;
                        }
                    } else {
                        throw std::runtime_error("Wrong ray!");
                    }
                    other = move(other, direction);
                    start |= other;
                }
                rookSlides[direction][y * 8 + x] = start ^ originalStart;
            }
        }
    }
}

void SlidingPieceRays::populateBishopSlides () {
    for (int direction = NORTH_EAST; direction <= NORTH_WEST; direction++) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Bitboard start = 1ULL << (8 * y + x);
                Bitboard originalStart = start;
                Bitboard other = start;
                while (true) {
                    if (direction == NORTH_EAST) {
                        if (start & (SquareMasks::eighthRank | SquareMasks::hFile)) {
                            break;
                        }
                    } else if (direction == SOUTH_EAST) {
                        if (start & (SquareMasks::firstRank | SquareMasks::hFile)) {
                            break;
                        }
                    } else if (direction == SOUTH_WEST) {
                        if (start & (SquareMasks::aFile | SquareMasks::firstRank)) {
                            break;
                        }
                    } else if (direction == NORTH_WEST) {
                        if (start & (SquareMasks::aFile | SquareMasks::eighthRank)) {
                            break;
                        }
                    } else {
                        throw std::runtime_error("Wrong ray!");
                    }
                    other = move(other, direction);
                    start |= other;
                }
                bishopSlides[direction][y * 8 + x] = start ^ originalStart;
            }
        }
    }
}

Bitboard SlidingPieceRays::getRookSlideAt (RayDirection direction, int square) {
    return rookSlides[direction][square];
}

Bitboard SlidingPieceRays::getBishopSlideAt (RayDirection direction, int square) {
    return bishopSlides[direction][square];
}

SlidingPieceRays::SlidingPieceRays () : rookSlides{}, bishopSlides{} {
    populateBishopSlides();
    populateRookSlides();
}

Bitboard SlidingPieceRays::getRookBlockerMask (int y, int x) {
    return getRookSlideAt(NORTH, 8 * y + x)
           | getRookSlideAt(EAST, 8 * y + x)
           | getRookSlideAt(SOUTH, 8 * y + x)
           | getRookSlideAt(WEST, 8 * y + x);
}

Bitboard SlidingPieceRays::getBishopBlockerMask (int y, int x) {
    return getBishopSlideAt(NORTH_EAST, 8 * y + x)
           | getBishopSlideAt(SOUTH_EAST, 8 * y + x)
           | getBishopSlideAt(SOUTH_WEST, 8 * y + x)
           | getBishopSlideAt(NORTH_WEST, 8 * y + x);
}

Bitboard SlidingPieceRays::getRookMoveBoard (Bitboard blockerBoard, int y, int x) {
    Bitboard result = 0;
    for (RayDirection direction = NORTH; direction <= WEST; ++direction) {
        Bitboard blockerMask = getRookSlideAt(direction, 8 * y + x);

        auto blockers = blockerMask & blockerBoard;

        if (blockers) {
            int firstBlockPosition;
            if (direction == NORTH || direction == EAST) {
                firstBlockPosition = __builtin_ctzl(blockers);
            } else if (direction == SOUTH || direction == WEST) {
                firstBlockPosition = 64 - __builtin_clzl(blockers) - 1;
            } else {
                throw std::runtime_error("Wrong direction, you probably messed up refactoring :)");
            }

            blockerMask ^= getRookSlideAt(direction, firstBlockPosition);
        }

        result |= blockerMask;
    }

    return result;
}

Bitboard SlidingPieceRays::getBishopMoveBoard (Bitboard blockerBoard, int y, int x) {
    Bitboard result = 0;
    for (RayDirection direction = NORTH_EAST; direction <= NORTH_WEST; ++direction) {
        Bitboard blockerMask = getBishopSlideAt(direction, 8 * y + x);

        auto blockers = blockerMask & blockerBoard;

        if (blockers) {
            int firstBlockPosition;
            if (direction == NORTH_EAST || direction == NORTH_WEST) {
                firstBlockPosition = __builtin_ctzl(blockers);
            } else if (direction == SOUTH_EAST || direction == SOUTH_WEST) {
                firstBlockPosition = 64 - __builtin_clzl(blockers) - 1;
            } else {
                throw std::runtime_error("Wrong direction, you probably messed up refactoring :)");
            }

            blockerMask ^= getBishopSlideAt(direction, firstBlockPosition);
        }

        result |= blockerMask;
    }

    return result;
}

