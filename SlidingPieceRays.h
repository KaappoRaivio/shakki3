//
// Created by kaappo on 22.11.2021.
//


#pragma once


#include "BitboardOperations.h"

class SlidingPieceRays {
private:
    Bitboard rookSlides[4][64];
    Bitboard bishopSlides[8][64];

    void populateRookSlides ();
    void populateBishopSlides ();

    Bitboard getRookSlideAt(RayDirection direction, int square);
    Bitboard getBishopSlideAt(RayDirection direction, int square);
public:
    Bitboard getRookBlockerMask(int y, int x);
    Bitboard getBishopBlockerMask(int y, int x);

    Bitboard getRookMoveBoard(Bitboard blockerBoard, int y, int x);
    Bitboard getBishopMoveBoard(Bitboard blockerBoard, int y, int x);

    SlidingPieceRays ();
};


