//
// Created by kaappo on 30.11.2021.
//

#include "BoardUtils.h"
#include "BitboardOperations.h"

#include "Board.h"
#include "Attacks.h"

using namespace BitboardOperations;

BoardState::BoardState (PieceColor turn, Move_raw previousMove, Piece capturedPiece, int plysSinceFiftyMoveReset, int wholeMoveCount)
        : plysSinceFiftyMoveReset{plysSinceFiftyMoveReset}, fullMoveCount{wholeMoveCount}, turn{turn}, previousMove{previousMove}, capturedPiece{capturedPiece} {}

std::ostream& operator<< (std::ostream& os, const BoardState& state) {
    os << "BoardState{plysSinceFiftyMoveReset: " << state.plysSinceFiftyMoveReset << " turn: " << state.turn << " previousMove: " << state.previousMove << " capturedPiece: " << state.capturedPiece << "}";
    return os;
}

BoardStateHistory::BoardStateHistory () : states{} {
    createNewFrame();
}

void BoardStateHistory::createNewFrame () {
    if (states.empty()) {
        states.push(BoardState{WHITE, Moves::NO_MOVE.raw(), Pieces::NO_PIECE, 0, 1});
    } else {
        states.push(BoardState{states.top()});
    }
}

const BoardState& BoardStateHistory::getCurrentFrame () const {
    return states.top();
}

const BoardState& BoardStateHistory::popFrame () {
    const BoardState& state = states.top();
    states.pop();
    return state;
}

void BoardStateHistory::pushState (BoardState newFrame) {
    states.push(newFrame);
}

void MoveGeneration::addBishopMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& bishops = context.getPieces()[color].boards[PieceTypes::BISHOP];
//    const Bitboard& occupancy = context.getPieces(WHITE).all | context.getPieces(color).all;

//    for (const Square& bishopSquare : bishops) {
//        const Bitboard& possibleSquares = Attacks::getInstance().getSlidingPieceAttackGenerator<PieceTypes::BISHOP>().getRaysToAllDirections(context, bishopSquare, color);
//
//        for (const Square& possibleSquare : possibleSquares) {
//            moves.emplace_back(context, bishopSquare, possibleSquare);
//        }
//    }
}

void MoveGeneration::addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& rooks = context.getPieces()[color].boards[PieceTypes::ROOK];
//    const Bitboard& occupancy = context.getPieces(WHITE).all | context.getPieces(BLACK).all;

//    for (const Square& rookSquare : rooks) {
//        const Bitboard& possibleSquares = Attacks::getInstance().getSlidingPieceAttackGenerator<PieceTypes::ROOK>().getRaysToAllDirections(context, rookSquare, color);
//
//        for (const Square& possibleSquare : possibleSquares) {
//            moves.emplace_back(context, rookSquare, possibleSquare);
//        }
//    }
}

void MoveGeneration::addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& knights = context.getPieces()[color].boards[PieceTypes::KNIGHT];

    for (const Square& knightSquare : knights) {
        const Bitboard& possibleSquares = Attacks::getInstance().getKnightAttackGenerator().getAttackAt(knightSquare)
                                          & ~context.getPieces(color).all;

        for (const Square& possibleSquare : possibleSquares) {
            moves.emplace_back(context, knightSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addPawnMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& occupancy = context.getPieces(WHITE).all | context.getPieces(BLACK).all;
    const Bitboard& pawns = context.getPieces()[color].boards[PieceTypes::PAWN];

    // pushes
    const Bitboard& pushes = Attacks::getInstance()
            .getPawnAttackGenerator()
            .getPawnPushes(occupancy, color, pawns);

    for (const Square& pawnSquare : pawns) {
        const auto& possiblePushSquares = Attacks::getInstance()
                .getPawnAttackGenerator()
                .getPossiblePushesOnEmptyBoard(color, pawnSquare);

        for (const Square& destinationSquare : pushes & possiblePushSquares) {
            moves.emplace_back(context, pawnSquare, destinationSquare);
        }
    }

    //captures
    const auto& opponentPieces = context.getPieces()[flip(color)].all;

    const Bitboard& captures = Attacks::getInstance()
            .getPawnAttackGenerator()
            .getPawnCaptures(context, pawns, color) & opponentPieces;

    for (const Square& pawnSquare : pawns) {
        const auto& possibleCaptureSquares = Attacks::getInstance()
                .getPawnAttackGenerator()
                .getPossibleCapturesOnEmptyBoard(color, pawnSquare);

        for (const Square& destinationSquare : captures & possibleCaptureSquares) {
            moves.emplace_back(context, pawnSquare, destinationSquare);
        }
    }
}

