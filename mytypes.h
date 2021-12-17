//
// Created by kaappo on 22.11.2021.
//

#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>

#define DEBUG true

typedef uint64_t Bitboard_raw;
typedef uint8_t RayDirection;

typedef uint16_t Move_raw;

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


//template <char SYMBOL, uint8_t INDEX>
struct PieceType {
    char symbol;
    uint8_t index;

public:
//    explicit constexpr PieceType (const char& symbol, const uint8_t& index);
//    constexpr PieceType () = default;


    char getSymbol (PieceColor color) const;
    operator int () const;

    bool operator== (const PieceType& rhs) const;

    bool operator!= (const PieceType& rhs) const;

    friend std::ostream& operator<< (std::ostream& os, const PieceType& type);

    std::string getSymbolFancy (PieceColor color) const;
};

namespace PieceTypes {
    constexpr PieceType KNIGHT {'n', 0};
    constexpr PieceType BISHOP {'b', 1};
    constexpr PieceType ROOK   {'r', 2};
    constexpr PieceType QUEEN  {'q', 3};
    constexpr PieceType PAWN   {'p', 4};
    constexpr PieceType KING   {'k', 5};
    constexpr PieceType NO_PIECE {'.', 255};
    const std::vector<PieceType> pieces = {KNIGHT, BISHOP, ROOK, QUEEN, PAWN, KING};
}

struct Piece {
    PieceType type;
    PieceColor color;

    Piece (PieceType type, PieceColor color);

    bool operator== (const Piece& rhs) const;
    bool operator!= (const Piece& rhs) const;

    operator bool () const;

    friend std::ostream& operator<< (std::ostream& os, const Piece& piece);
};

namespace Pieces {
    extern Piece NO_PIECE;

    Piece parsePiece (char asChar);
}

PieceColor flip (PieceColor color);