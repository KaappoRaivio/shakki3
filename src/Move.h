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
#define SCORES boost::container::static_vector<int, 128>


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

    Move () : Move{true} {}
    Move (Move_raw move);
    Move (const Board& context, const Square& from, const Square& to, const PieceType& pieceToPromoteTo = PieceTypes::NO_PIECE);
    Move (const Move& other) = default;

    Square getOrigin () const;
    Square getDestination () const;
    const Piece& getMovingPiece (const Board& context) const;
    bool isCapture () const;
    bool isPromotion () const;
    PieceType getPromotedPiece () const;
    bool isCastling (CastlingSide SIDE) const;
    CastlingStatus getNewCastlingStatus (const Board& context, const CastlingStatus& oldStatus, const Piece& possiblyCapturedPiece) const;
    bool isEnPassant () const;
    bool isDoublePawnPush () const;
    Move_raw raw () const;

    bool operator== (const Move& rhs) const;
    bool operator!= (const Move& rhs) const;
    operator int () const;
    friend std::ostream& operator<< (std::ostream& os, const Move& move);

    static Move fromString (std::string moveString, const Board& context);
};

namespace Moves {
    constexpr Move NO_MOVE{true};
}


