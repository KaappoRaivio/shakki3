//
// Created by kaappo on 25.11.2021.
//

#include "Square.h"
#include "BitboardOperations.h"

namespace Directions = BitboardOperations::Directions;

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
    os << (char) (square.getX() + 65) << (int) square.getY() + 1;
    return os;
}

Square::operator int () const {
    return value;
}

RayDirection Square::getDirection (const Square& other, const PieceType& type) const {
    if (type == PieceTypes::BISHOP) {
        if (other.getX() > getX()) {
            if (other.getY() > getY()) {
                return Directions::NORTH_EAST;
            } else {
                return Directions::SOUTH_EAST;
            }
        } else {
            if (other.getY() > getY()) {
                return Directions::NORTH_WEST;
            } else {
                return Directions::SOUTH_WEST;
            }
        }
    } else if (type == PieceTypes::ROOK) {
        if (other.getX() > getX()) return Directions::EAST;
        else if (other.getX() < getX()) return Directions::WEST;
        else if (other.getY() > getY()) return Directions::NORTH;
        else if (other.getY() < getY()) return Directions::SOUTH;
        else {
            std::cerr << "This: " << *this << ", other: " << other << std::endl;
            throw std::runtime_error("cannot convert!");
        }
    } else {
        std::cerr << "Invalid piece TYPE " << type << std::endl;
        throw std::runtime_error("Invalid piece TYPE!");
    }
}

Square Square::move (RayDirection direction) {
    auto shift = BitboardOperations::rayDirectionToShift(direction, WHITE);
    return *this;
}


Square Square::move (RayDirection direction, PieceColor perspective) const {
    auto shift = BitboardOperations::rayDirectionToShift(direction, perspective);

    uint8_t newValue = value + shift;

    return {newValue};
}

