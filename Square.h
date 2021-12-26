//
// Created by kaappo on 25.11.2021.
//


#pragma once


#include <cstdint>
#include <ostream>
#include "mytypes.h"

struct Square {
    static const uint8_t INVALID = 255;

    uint8_t value;

public:
    Square (Square_raw square);
    Square (int square);
    Square (int y, int x);

    uint8_t getX () const;
    uint8_t getY () const;
    uint8_t getValue () const;
    bool isInvalid () const;

//    const Square& operator+= (const Square& other);
//    friend Square operator+ (Square square, const Square& other);
//
//    const Square& operator-= (const Square& other);
//    friend Square operator- (Square square, Square other);

    operator int () const;

    uint8_t diffY (Square square) const;
    RayDirection getDirection (const Square& other, const PieceType& type) const;

    friend std::ostream& operator<< (std::ostream& os, const Square& square);

    Square move (RayDirection direction, PieceColor perspective) const;
    Square move (RayDirection direction);
};


