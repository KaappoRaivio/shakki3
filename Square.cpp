//
// Created by kaappo on 25.11.2021.
//

#include "Square.h"
#include "BitboardOperations.h"

Square flip ();

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

//const Square& Square::operator+= (const Square& other) {
//    value += other.value;
//    return *this;
//}
//
//Square operator+ (Square square, const Square& other) {
//    std::cout << square << other << std::endl;
//
//    square += other;
//
//    std::cout << square << other << std::endl;
//
//    return square;
//}
//
//const Square& Square::operator-= (const Square& other) {
//    value -= other.value;
//    return *this;
//}
//
//Square operator- (Square square, Square other) {
//    square += other;
//
//    return square;
//}

uint8_t Square::diffY (Square square) const {
    return std::max(getY(), square.getY()) - std::min(getY(), square.getY());
}

uint8_t Square::diffX (Square square) const {
    return std::max(getX(), square.getX()) - std::min(getX(), square.getX());
}

std::ostream& operator<< (std::ostream& os, const Square& square) {
    os << (char) (square.getX() + 'A') << (int) square.getY() + 1;
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

Square Square::move (RayDirection direction) const {
    return move(direction, WHITE);
}


Square Square::move (RayDirection direction, PieceColor perspective) const {
    uint8_t offX;
    uint8_t offY;
    namespace Directions = BitboardOperations::Directions;
    switch (direction) {
        case Directions::NORTH_EAST:
            offX = 1;
            offY = 1;
            break;
        case Directions::NORTH_WEST:
            offX = -1;
            offY = 1;
            break;
        case Directions::NORTH:
            offY = 1;
            offX = 0;
            break;
        case Directions::SOUTH_EAST:
            offX = 1;
            offY = -1;
            break;
        case Directions::SOUTH_WEST:
            offX = -1;
            offY = -1;
            break;
        case Directions::SOUTH:
            offY = -1;
            offX = 0;
            break;
        case Directions::EAST:
            offX = 1;
            offY = 0;
            break;
        case Directions::WEST:
            offX = -1;
            offY = 0;
            break;
        default:
            throw std::runtime_error(":)))))");
    }

    if (perspective == BLACK) {
        offX *= -1;
        offY *= -1;
    }

    uint8_t newX = getX() + offX;
    uint8_t newY = getY() + offY;

    if (newX < 0 || newX >= 8) return INVALID;
    if (newY < 0 || newY >= 8) return INVALID;

    if (DEBUG) {
//        if (newValue != value + shift) {
//            std::cerr
//                    << "WARNING!!! Square move overflow: trying to move "
//                    << *this << " (" << +value << ") by "
//                    << +shift << " amount!" << std::endl;
//        }
    }

    return {newY, newX};
}

bool Square::isInvalid () const {
    return value == 255;
}

Square Square::asColorRotate (PieceColor color) const {
    if (color == WHITE) return *this;
    if (color == BLACK) return rotate180();
}

Square Square::asColorFlip (PieceColor color) const {
    if (color == WHITE) return *this;
    if (color == BLACK) return flip();
}


Square Square::rotate180 () const {
    return {63 - value};
}

Square Square::flip () const {
    return {7 - getY(), getX()};
}

Square Square::fromString (std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::toupper);
    int x = string.at(0) - 'A';
    int y = string.at(1) - '0' - 1;

    return {y, x};
}

