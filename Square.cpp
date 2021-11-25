//
// Created by kaappo on 25.11.2021.
//

#include "Square.h"

Square::Square (Square_raw square) : value{static_cast<uint8_t>(square)} {

}

Square::Square (int y, int x) : value{static_cast<uint8_t>(8 * y + x)} {

}

Square::Square (int square) : value{static_cast<uint8_t>(square)} {

}

uint8_t Square::getX () const {
    return value % 8;
}

uint8_t Square::getY () const {
    return value >> 3;
}

uint8_t Square::getValue () const {
    return value;
}
