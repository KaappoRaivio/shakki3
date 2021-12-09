//
// Created by kaappo on 30.11.2021.
//

#include "Attacks.h"
#include "BoardUtils.h"
#include "SlidingPieceRays.h"
#include "KnightAttacks.h"
//#include "Board.h"

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
    const Bitboard& occupancy = context.getPieceSet(WHITE).all | context.getPieceSet(color).all;

    for (const Square& bishopSquare : bishops) {
        const Bitboard& possibleSquares = Attacks::getInstance().getSlidingPieceAttackGenerator().getBishopMoveBoard(context, bishopSquare, color);

        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, bishopSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& rooks = context.getPieces()[color].boards[PieceTypes::ROOK];
    const Bitboard& occupancy = context.getPieceSet(WHITE).all | context.getPieceSet(BLACK).all;

    for (const Square& rookSquare : rooks) {
        const Bitboard& possibleSquares = Attacks::getInstance().getSlidingPieceAttackGenerator().getRookMoveBoard(context, rookSquare, color);

//                .getRookMoveBoard(occupancy, rookSquare);
        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, rookSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& knights = context.getPieces()[color].boards[PieceTypes::KNIGHT];

    for (const Square& knightSquare : knights) {
        const Bitboard& possibleSquares = Attacks::getInstance().getKnightAttackGenerator().getAttackAt(knightSquare)
                                          & ~context.getPieceSet(color).all;

        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, knightSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addPawnMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& occupancy = context.getPieceSet(WHITE).all | context.getPieceSet(BLACK).all;
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
    const Bitboard& captures = Attacks::getInstance()
            .getPawnAttackGenerator()
            .getPawnCaptures(context, pawns, color);

    for (const Square& pawnSquare : pawns) {
        const auto& possibleCaptureSquares = Attacks::getInstance()
                .getPawnAttackGenerator()
                .getPossibleCapturesOnEmptyBoard(color, pawnSquare);

        for (const Square& destinationSquare : captures & possibleCaptureSquares) {
            moves.emplace_back(context, pawnSquare, destinationSquare);
        }
    }
}

Bitboard BoardAnalysis::getAttackMask (const Board& board, PieceColor color) {
    Bitboard attackMask;

    const Attacks& attacks = Attacks::getInstance();
//
//    attackMask |= attacks.getPawnAttackGenerator().getPawnCaptures(board, board.getPieces()[color].boards[PieceTypes::PAWN], color);
//    attackMask |= attacks.getSlidingPieceAttackGenerator().getBishopMoveBoard(board, board.getPieces()[color].boards[PieceTypes::BISHOP], color);
//    attackMask |= attacks.getSlidingPieceAttackGenerator().getRookMoveBoard(board, board.getPieces()[color].boards[PieceTypes::ROOK], color);
//    attackMask |= attacks.getSlidingPieceAttackGenerator().getQueenMoveBoard(board, board.getPieces()[color].boards[PieceTypes::QUEEN], color);
    attackMask |= attacks.getKnightAttackGenerator().getAttackAt(board.getPieces()[color].boards[PieceTypes::KNIGHT]);
//    attackMask |= attacks.getKingAttackGenerator().getKingAttackAt(board, board.getPieces()[color].boards[PieceTypes::KING].ls1b(), color);

    return attackMask;
}

bool BoardAnalysis::isSquareAttacked (const Board& board, const Square& square, PieceColor color) {
    return bool(getAttackMask(board, color) & square);
}
