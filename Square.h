//
// Created by kaappo on 25.11.2021.
//


#pragma once


#include <cstdint>
#include "mytypes.h"

class Square {
private:
    uint8_t value;

public:
    explicit Square (Square_raw square);
    explicit Square (int square);
    Square (int y, int x);

    uint8_t getX () const;
    uint8_t getY () const;
    uint8_t getValue () const;
};


