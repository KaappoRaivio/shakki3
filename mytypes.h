//
// Created by kaappo on 22.11.2021.
//

#pragma once

#include <cstdint>
#include <iostream>

typedef uint64_t Bitboard_raw;
typedef uint8_t RayDirection;

enum Square_raw {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

enum PieceColor {
    WHITE = 0, BLACK = 1, EMPTY = 2
};

enum PieceType {
    KNIGHT = 0,
    BISHOP = 1,
    ROOK = 2,
    QUEEN = 3,
    PAWN = 4,
    KING = 5
};
static char pieceTypeToSymbol (PieceType pieceType) {
    switch (pieceType) {
        case ROOK: return 'r';
        case BISHOP: return 'b';
        case KNIGHT: return 'n';
        case QUEEN: return 'q';
        case KING: return 'k';
        case PAWN: return 'p';
        default: return 'x';
    }
}

static char pieceTypeToSymbol (int pieceType) {
    if (pieceType >= 0 && pieceType < 7) return pieceTypeToSymbol(static_cast<PieceType>(pieceType));
    else throw std::runtime_error("Invalid pieceType!");
}


static PieceColor flip (const PieceColor color) {
    switch (color) {
        case WHITE:
            return BLACK;
        case BLACK:
            return WHITE;
        case EMPTY:
            std::cerr << "Probably shouldn't invert EMPTY color!" << std::endl;
            return EMPTY;
        default:
            throw std::runtime_error("Shouldn't happen!");
    }
}