//
// Created by kaappo on 24.11.2021.
//


#pragma once


#include <cstdint>
#include <ostream>
#include "mytypes.h"
#include "Square.h"



class Bitboard {
private:
    Bitboard_raw value;
public:
    Bitboard (uint64_t value); // NOLINT(google-explicit-constructor)
    Bitboard (Square_raw square); // NOLINT(google-explicit-constructor)
    Bitboard (Square square); // NOLINT(google-explicit-constructor)
    Bitboard ();

    Bitboard move (RayDirection direction) const;
    Bitboard move (RayDirection direction, PieceColor perspective) const;
    Bitboard move (RayDirection direction, PieceColor perspective, int amount) const;
    Bitboard move (RayDirection direction, PieceColor perspective, int amount, bool wrapAround) const;

    uint8_t ls1b () const;
    uint8_t ms1b () const;

    std::string& burnTo (std::string& buffer, char symbol='1') const;

    Bitboard flipVertical () const;
    Bitboard rotate180 () const;

    Bitboard asColor (PieceColor color) const;

    Bitboard& operator<<=(int shift);
    friend Bitboard operator<< (Bitboard bitboard, int shift);

    Bitboard& operator>>=(int shift);
    friend Bitboard operator>> (Bitboard bitboard, int shift);

    Bitboard& operator&=(Bitboard other);
    friend Bitboard operator& (Bitboard bitboard, Bitboard other);

    Bitboard& operator|=(Bitboard other);
    friend Bitboard operator| (Bitboard bitboard, Bitboard other);

    Bitboard& operator^=(Bitboard other);
    friend Bitboard operator^ (Bitboard bitboard, Bitboard other);

    bool operator== (const Bitboard& rhs) const;

    bool operator!= (const Bitboard& rhs) const;

    friend Bitboard operator~ (Bitboard bitboard);
    explicit operator bool () const;
    explicit operator unsigned long long () const;

    friend std::ostream& operator<< (std::ostream& os, const Bitboard& bitboard);


    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Square;
        using pointer           = Square*;
        using reference         = Square&;

        reference operator*() {
//            square = Square{__builtin_ctzl(bitboard)};
            square.value = __builtin_ctzl(bitboard);
            return square;
        }
        pointer operator->() {
            square.value = __builtin_ctzl(bitboard);
//            square = Square{__builtin_ctzl(bitboard)};
            return &square;
        }

        Iterator& operator++() { bitboard ^= bitboard & -bitboard; return *this;}
        Iterator operator++(int) { const Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.bitboard == b.bitboard; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.bitboard != b.bitboard; };


        explicit Iterator (const Bitboard& bitboard) : bitboard{bitboard.value}, square{bitboard.ls1b()} { }

    private:
        Bitboard_raw bitboard;
        value_type square;
    };

    Iterator begin () const;
    static Iterator end () ;
};


