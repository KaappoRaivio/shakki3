//
// Created by kaappo on 20.11.2021.
//


#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>
#include "PieceSet.h"


class Board {
public:
    Board ();

    friend std::ostream& operator<< (std::ostream& os, const Board& board);
private:
    const PieceSet white;
    const PieceSet black;
};


