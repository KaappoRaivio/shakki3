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

const Square& Square::operator+= (const Square& other) {
    value <<= 8 * other.getY();
    value <<= other.getX();

    return *this;
}

Square operator+ (Square square, const Square& other) {
    square += other;
    return square;
}

const Square& Square::operator-= (const Square& other) {
    value >>= 8 * other.getY();
    value >>= other.getX();

    return *this;
}

Square operator- (Square square, Square other) {
    square += other;

    return square;
}

uint8_t Square::diffY (Square square) const {
    return std::abs((int8_t) getY() - (int8_t) square.getY());
}

std::ostream& operator<< (std::ostream& os, const Square& square) {
    os << (char)(square.getX() + 65) << (int) square.getY() + 1;
    return os;
}

Square::operator int () const {
    return value;
}

