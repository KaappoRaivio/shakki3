//
// Created by kaappo on 24.11.2021.
//

#include "Bitboard.h"
#include "BitboardOperations.h"
#include "MyMath.h"

Bitboard::Bitboard (uint64_t value) : value{value} {

}

Bitboard& Bitboard::operator<<= (int shift) {
    if (shift > 0) {
        value <<= shift;
    } else if (shift < 0) {
        value >>= shift;
    }

    return *this;
}

Bitboard operator<< (Bitboard bitboard, const int shift) {
    bitboard <<= shift;
    return bitboard;
}

Bitboard& Bitboard::operator>>= (int shift) {
    if (shift > 0) {
        value >>= shift;
    } else if (shift < 0) {
        value <<= shift;
    }

    return *this;
}

Bitboard operator>> (Bitboard bitboard, int shift) {
    bitboard >>= shift;
    return bitboard;
}

Bitboard& Bitboard::operator&= (Bitboard other) {
    value &= other.value;
    return *this;
}

Bitboard operator& (Bitboard bitboard, Bitboard other) {
    bitboard &= other;
    return bitboard;
}

Bitboard& Bitboard::operator|= (Bitboard other) {
    value |= other.value;
    return *this;
}

Bitboard operator| (Bitboard bitboard, Bitboard other) {
    bitboard |= other;
    return bitboard;
}

Bitboard& Bitboard::operator^= (Bitboard other) {
    value ^= other.value;
    return *this;
}

Bitboard operator^ (Bitboard bitboard, Bitboard other) {
    bitboard ^= other;
    return bitboard;
}

Bitboard operator~ (Bitboard bitboard) {
    bitboard.value = ~bitboard.value;
    return bitboard;
}

Bitboard Bitboard::move (RayDirection direction, PieceColor perspective, int amount, bool wrapAround) const {
    Bitboard_raw b = value;

    int8_t shift = BitboardOperations::rayDirectionToShift(direction, perspective);

    Bitboard_raw bitmask;
    if (wrapAround) {
        bitmask = 0;
    } else {
        if (MyMath::modulus(shift, 8) == 7) {
            bitmask = BitboardOperations::SquareMasks::fileH.value;
        } else if (MyMath::modulus(shift, 8) == 1) {
            bitmask = BitboardOperations::SquareMasks::fileA.value;
        } else {
            std::cerr << "Warning: wrapAround is false, but shift is more than one (" << +shift << "). Unable to mask of irrelevant bits!!" << std::endl;
            bitmask = 0;
        }
    }

    for (int i = 0; i < amount; i++) {
        b = BitboardOperations::shift(b, shift) & ~bitmask;
        if (b == 0) return b;
    }

    return b;

}

Bitboard::Bitboard () : value{0} {

}

Bitboard::operator bool () const {
    return value != 0;
}

Bitboard Bitboard::move (RayDirection direction, PieceColor perspective, int amount) const {
    return move(direction, perspective, amount, true);
}

Bitboard Bitboard::move (RayDirection direction, PieceColor perspective) const {
    return move(direction, perspective, 1);
}

std::ostream& operator<< (std::ostream& os, const Bitboard& bitboard) {
    return os << "Bitboard(" << bitboard.value << ") {" << BitboardOperations::bitboardToString(bitboard.value) << "\n}";
}

uint8_t Bitboard::ls1b () const {
    return __builtin_ctzl(value);
}

uint8_t Bitboard::ms1b () const {
    return 64 - __builtin_clzl(value) - 1;
}

Bitboard Bitboard::flipVertical () const {
    return BitboardOperations::flipVertical(value);
}

std::string& Bitboard::burnTo (std::string& buffer, char symbol) const {
    auto pieces = value;
    while (pieces != 0) {
        buffer[__builtin_ctzl(pieces)] = symbol;
        pieces &= pieces - 1;
    }

    return buffer;
}

Bitboard Bitboard::rotate180 () const {
    const Bitboard_raw h1 = 0x5555555555555555ULL;
    const Bitboard_raw h2 = 0x3333333333333333ULL;
    const Bitboard_raw h4 = 0x0F0F0F0F0F0F0F0FULL;
    const Bitboard_raw v1 = 0x00FF00FF00FF00FFULL;
    const Bitboard_raw v2 = 0x0000FFFF0000FFFFULL;

    Bitboard_raw x = value;

    x = ((x >> 1) & h1) | ((x & h1) << 1);
    x = ((x >> 2) & h2) | ((x & h2) << 2);
    x = ((x >> 4) & h4) | ((x & h4) << 4);
    x = ((x >> 8) & v1) | ((x & v1) << 8);
    x = ((x >> 16) & v2) | ((x & v2) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}

Bitboard Bitboard::asColor (PieceColor color) const {
    if (color == BLACK) return rotate180();
    else return *this;
}

Bitboard::Bitboard (Square_raw square) : Bitboard(1 << square) {

}

Bitboard Bitboard::move (RayDirection direction) const {
    return move(direction, WHITE);
}

Bitboard::Bitboard (Square square) : Bitboard(1ull << square.getValue()) {

}

Bitboard::Iterator Bitboard::begin () const {
    return Bitboard::Iterator(*this);
}

Bitboard::Iterator Bitboard::end () {
    return Bitboard::Iterator(0);
}

Bitboard::operator unsigned long long () const {
    return value;
}
