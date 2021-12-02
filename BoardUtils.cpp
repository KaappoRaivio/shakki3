//
// Created by kaappo on 30.11.2021.
//

#include "Attacks.h"
#include "BoardUtils.h"
#include "SlidingPieceRays.h"
#include "KnightAttacks.h"
//#include "Board.h"

BoardState::BoardState (PieceColor turn, Move_raw previousMove, Piece capturedPiece, int plysSinceFiftyMoveReset)
        : plysSinceFiftyMoveReset{plysSinceFiftyMoveReset}, turn{turn}, previousMove{previousMove}, capturedPiece(capturedPiece) {}

std::ostream& operator<< (std::ostream& os, const BoardState& state) {
    os << "BoardState{plysSinceFiftyMoveReset: " << state.plysSinceFiftyMoveReset << " turn: " << state.turn << " previousMove: " << state.previousMove << " capturedPiece: " << state.capturedPiece << "}";
    return os;
}

BoardStateHistory::BoardStateHistory () : states{} {
    createNewFrame();
}

void BoardStateHistory::createNewFrame () {
    if (states.empty()) {
        states.push(BoardState{WHITE, Moves::NO_MOVE.raw(), Pieces::NO_PIECE, 0});
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
    const Bitboard& occupancy = context.getPieceSet(WHITE).all | context.getPieceSet(BLACK).all;

    for (const Square& bishopSquare: bishops) {
        const Bitboard& possibleSquares = Attacks::getInstance().getSlidingPieceAttackGenerator().getBishopMoveBoard(occupancy, bishopSquare)
                                          & ~context.getPieceSet(color).all;
        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, bishopSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& rooks = context.getPieces()[color].boards[PieceTypes::ROOK];
    const Bitboard& occupancy = context.getPieceSet(WHITE).all | context.getPieceSet(BLACK).all;

    for (const Square& rookSquare: rooks) {
        const Bitboard& possibleSquares = Attacks::getInstance().getSlidingPieceAttackGenerator().getRookMoveBoard(occupancy, rookSquare)
                                          & ~context.getPieceSet(color).all;

//                .getRookMoveBoard(occupancy, rookSquare);
        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, rookSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& knights = context.getPieces()[color].boards[PieceTypes::KNIGHT];

    for (const Square& knightSquare: knights) {
//        const Bitboard& possibleSquares = SlidingPieces::instance.getRookMoveBoard(occupancy, knightSquare);
        const Bitboard& possibleSquares = Attacks::getInstance().getKnightAttackGenerator().getAttackAt(knightSquare)
                                          & ~context.getPieceSet(color).all;

        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, knightSquare, possibleSquare);
        }
    }
}
