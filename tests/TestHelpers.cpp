//
// Created by kaappo on 29.12.2021.
//
#include "catch2/catch.hpp"
#include "TestHelpers.h"

namespace TestHelpers {
    void verify (const Move& previousMove, const Board& board) {
        REQUIRE(board.getPieceAt(previousMove.getDestination()) == previousMove.getMovingPiece(board));
        REQUIRE(board.getPieceAt(previousMove.getOrigin()) == Pieces::NO_PIECE);

        REQUIRE(Move{board.getHistory()->getCurrentFrame().previousMove} == previousMove);

        PieceColor color = previousMove.getMovingPiece(board).color;
        if (previousMove.isCastling(MoveBitmasks::KING_CASTLE)) {
            REQUIRE(board.getHistory()->getCurrentFrame().castlingStatus.canCastle(color, MoveBitmasks::KING_CASTLE) == false);
        }

        if (previousMove.isCastling(MoveBitmasks::QUEEN_CASTLE)) {
            REQUIRE(board.getHistory()->getCurrentFrame().castlingStatus.canCastle(color, MoveBitmasks::QUEEN_CASTLE) == false);
        }
    }
}
