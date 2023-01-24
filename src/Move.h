//
// Created by kaappo on 25.11.2021.
//


#pragma once


#include <cstdint>
#include <ostream>
#include "mytypes.h"
#include "Square.h"
#include "CastlingStatus.h"
//#include "BoardUtils.h"
//#include "Board.h"
#include "boost/container/static_vector.hpp"

//#define MOVES std::vector<Move>
#define MOVES boost::container::static_vector<Move, 128>


// 000000 000000 0         0       0        0
// from   to     promotion capture special1 special2
namespace MoveBitmasks {
    constexpr int CAPTURE = 1 << 2;
    constexpr int PROMOTION = 1 << 3;

    constexpr int SPECIAL_MASK = 0b11;
}

namespace Specials {
    constexpr int DOUBLE_PAWN_PUSH = 1;
    constexpr int KING_CASTLE = 2;
    constexpr int QUEEN_CASTLE = 3;
    constexpr int EN_PASSANT = 1;
}


class Board;

class Move {
private:
    Move_raw move;

public:
    constexpr explicit Move (bool NO_MOVE) {
        if (NO_MOVE) {
            move = 0xffffull;
        } else {
            throw std::runtime_error("NO_MOVE must be true!");
        }

    };

    Move (Move_raw move);

    Move (const Board& context, const Square& from, const Square& to, const PieceType& pieceToPromoteTo = PieceTypes::NO_PIECE);

    Move (const Move& other) = default;

    Square getOrigin () const;

    Square getDestination () const;

    bool isCapture () const;

    bool isPromotion () const;

//    requires IsCastlingSide<SIDE>
//    template<CastlingSide SIDE>
    bool isCastling (CastlingSide SIDE) const {
        if (!isPromotion() && !isCapture()) {
            return (move & MoveBitmasks::SPECIAL_MASK) == SIDE;
//            if (SIDE == Specials::KING_CASTLE && ((move & 0b11) == Specials::KING_CASTLE)) {
//                return true;
//            }
//            if (SIDE == Specials::QUEEN_CASTLE && ((move & 0b11) == Specials::QUEEN_CASTLE)) {
//                return true;
//            }
        }

        return false;
    }

    bool operator== (const Move& rhs) const;

    bool operator!= (const Move& rhs) const;

    friend std::ostream& operator<< (std::ostream& os, const Move& move);

    Move_raw raw () const;

    const Piece& getMovingPiece (const Board& context) const;

    CastlingStatus getNewCastlingStatus (const Board& context, const CastlingStatus& oldStatus, const Piece& possiblyCapturedPiece) const;

    PieceType getPromotedPiece () const;

    static Move fromString (std::string moveString, const Board& context);

    bool isEnPassant () const;

    bool isDoublePawnPush () const;
};

namespace Moves {
    constexpr Move NO_MOVE{true};
}


