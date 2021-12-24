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

void MoveGeneration::addBishopMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12) {
    const Bitboard& bishops = context.getPieces()[color].boards[PieceTypes::BISHOP];
    const Bitboard& occupancy = context.getPieces(WHITE).all | context.getPieces(color).all;


    Bitboard possiblePinnedSquares;
    if (pinMaskD12 == 0) {
        possiblePinnedSquares = -1;
    } else {
        possiblePinnedSquares = pinMaskD12;
    }

    for (const Square& bishopSquare: bishops) {
        if (bishopSquare & pinMaskHV) continue; // pinned bishops can't move if pinned vertically or horizontally

        Bitboard possibleSquares = Attacks::getInstance()
                                                  .getBishopAttacks()
                                                  .getRaysToAllDirections(context, bishopSquare, color)
                                          & checkMask;
//                                          & possiblePinnedSquares;
        if (bishopSquare & pinMaskD12) possibleSquares &= possiblePinnedSquares;


        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, bishopSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12) {
    const Bitboard& rooks = context.getPieces()[color].boards[PieceTypes::ROOK];
    const Bitboard& occupancy = context.getPieces(WHITE).all | context.getPieces(BLACK).all;

    Bitboard possiblePinnedSquares;
    if (pinMaskHV == 0) {
        possiblePinnedSquares = -1;
    } else {
        possiblePinnedSquares = pinMaskHV;
    }

    for (const Square& rookSquare: rooks) {
        if (rookSquare & pinMaskD12) continue;  // pinned rooks can't move if pinned diagonally
        Bitboard possibleSquares = Attacks::getInstance()
                                                  .getRookAttacks()
                                                  .getRaysToAllDirections(context, rookSquare, color)
                                          & checkMask;

        if (rookSquare & pinMaskHV) possibleSquares &= possiblePinnedSquares;

        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, rookSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addQueenMoves (std::vector<Move>& moves, const Board& context, PieceColor color) {
    const Bitboard& queens = context.getPieces()[color].boards[PieceTypes::QUEEN];
    const Bitboard& occupancy = context.getPieces(WHITE).all | context.getPieces(BLACK).all;

    for (const Square& queenSquare: queens) {
        const Bitboard& possibleSquares = Attacks::getInstance().getQueenAttacks().getRaysToAllDirections(context, queenSquare, color);

        for (const Square& possibleSquare: possibleSquares) {
            moves.emplace_back(context, queenSquare, possibleSquare);
        }
    }
}

void MoveGeneration::addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMask) {
    const Bitboard& knights = context.getPieces()[color].boards[PieceTypes::KNIGHT];

    for (const Square& knightSquare: knights) {
        if (knightSquare & pinMask) continue; // pinned knights can't move

        const Bitboard& possibleSquares = Attacks::getInstance().getKnightAttackGenerator().getAttackAt(knightSquare)
                                          & ~context.getBlockers(color)
                                          & checkMask;

        for (const Square& possibleSquare: possibleSquares) {
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

    for (const Square& pawnSquare: pawns) {
        const auto& possiblePushSquares = Attacks::getInstance()
                .getPawnAttackGenerator()
                .getPossiblePushesOnEmptyBoard(color, pawnSquare);

        for (const Square& destinationSquare: pushes & possiblePushSquares) {
            moves.emplace_back(context, pawnSquare, destinationSquare);
        }
    }

    //captures
    const auto& opponentPieces = context.getPieces()[flip(color)].all;

    const Bitboard& captures = Attacks::getInstance()
                                       .getPawnAttackGenerator()
                                       .getPawnCaptures(context, pawns, color) & opponentPieces;

    for (const Square& pawnSquare: pawns) {
        const auto& possibleCaptureSquares = Attacks::getInstance()
                .getPawnAttackGenerator()
                .getPossibleCapturesOnEmptyBoard(color, pawnSquare);

        for (const Square& destinationSquare: captures & possibleCaptureSquares) {
            moves.emplace_back(context, pawnSquare, destinationSquare);
        }
    }
}

void MoveGeneration::addKingMoves (std::vector<Move>& moves, const Board& context, PieceColor color, Bitboard checkMask) {
    const Square& kingSquare = context.getPieces()[color].boards[PieceTypes::KING].ls1b();

    if (checkMask == -1) checkMask = 0;

    const Bitboard& possibleSquares = Attacks::getInstance().getKingAttackGenerator().getKingAttackAt(context, kingSquare, color)
                                      & ~context.getBlockers(color)
                                      & ~checkMask;

    for (const Square& possibleSquare: possibleSquares) {
        moves.emplace_back(context, kingSquare, possibleSquare);
    }
}

