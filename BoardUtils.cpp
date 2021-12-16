//
// Created by kaappo on 30.11.2021.
//

#include "BoardUtils.h"

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

Bitboard BoardAnalysis::getAttackMask (const Board& board, PieceColor color) {
    Bitboard attackMask;

    const Attacks& attacks = Attacks::getInstance();
//
    attackMask |= attacks.getPawnAttackGenerator().getPawnCaptures(board, board.getPieces()[color].boards[PieceTypes::PAWN], color);
//    attackMask |= attacks.getSlidingPieceAttackGenerator<PieceTypes::BISHOP>().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::BISHOP], color);
    attackMask |= attacks.getBishopAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::BISHOP], color);
//    attackMask |= attacks.getSlidingPieceAttackGenerator<PieceTypes::ROOK>().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::ROOK], color);
//    attackMask |= attacks.getSlidingPieceAttackGenerator<PieceTypes::QUEEN>().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::QUEEN], color);
    attackMask |= attacks.getKnightAttackGenerator().getAttackAt(board, board.getPieces()[color].boards[PieceTypes::KNIGHT], color);
    attackMask |= attacks.getKingAttackGenerator().getKingAttackAt(board, board.getPieces()[color].boards[PieceTypes::KING].ls1b(), color);

    return attackMask;
}

bool BoardAnalysis::isSquareAttacked (const Board& board, const Square& square, PieceColor color) {
    return bool(getAttackMask(board, color) & square);
}

Bitboard BoardAnalysis::getCheckMask (const Board& context, PieceColor const color) {
    Bitboard checkMask;
    const Attacks& attacks = Attacks::getInstance();

    const Square& kingPosition = context.getPieces(color).boards[PieceTypes::KING].ls1b();

    // bishop and queen
//    const Bitboard& supposedBishops = attacks.getSlidingPieceAttackGenerator<PieceTypes::BISHOP>().getRaysToAllDirections(context, kingPosition, color);
//    for (const Square& possibleBishop : supposedBishops) {
//        if (context.getPieceAt(possibleBishop) == Piece{PieceTypes::BISHOP, flip(color)}
//            || context.getPieceAt(possibleBishop) == Piece{PieceTypes::QUEEN, flip(color)}) {
//
//            RayDirection direction = kingPosition.getDirection(possibleBishop, PieceTypes::BISHOP);
//            checkMask |= attacks.getSlidingPieceAttackGenerator<PieceTypes::BISHOP>().getRayTo(context, kingPosition, direction);
//        }
//    }

    // rook and queen
//    const Bitboard& supposedRooks = attacks.getSlidingPieceAttackGenerator<PieceTypes::ROOK>().getRaysToAllDirections(context, kingPosition, color);
//    for (const Square& possibleRook : supposedRooks) {
//        if (context.getPieceAt(possibleRook) == Piece{PieceTypes::ROOK, flip(color)}
//            || context.getPieceAt(possibleRook) == Piece{PieceTypes::QUEEN, flip(color)}) {
//            RayDirection direction = kingPosition.getDirection(possibleRook, PieceTypes::ROOK);
//            checkMask |= attacks.getSlidingPieceAttackGenerator<PieceTypes::ROOK>().getRayTo(context, kingPosition, direction);
//        }
//    }

    // knight
    const Bitboard& supposedKnights = attacks.getKnightAttackGenerator().getAttackAt(context, kingPosition, color);
    for (const Square& possibleKnight : supposedKnights) {
        if (context.getPieceAt(possibleKnight) == Piece{PieceTypes::KNIGHT, flip(color)}) {
            checkMask |= possibleKnight;
        }
    }

    // pawn
    const Square& possiblePawn1 = kingPosition.move(Directions::SOUTH_EAST, flip(color));
    if (context.getPieceAt(possiblePawn1) == Piece{PieceTypes::PAWN, flip(color)}) {
        checkMask |= possiblePawn1;
    }

    const Square& possiblePawn2 = kingPosition.move(Directions::SOUTH_WEST, flip(color));
    if (context.getPieceAt(possiblePawn2) == Piece{PieceTypes::PAWN, flip(color)}) {
        checkMask |= possiblePawn2;
    }



    return checkMask;
}

Bitboard BoardAnalysis::getPinMask (const Board& board, PieceColor color) {
    const Square& kingPosition = board.getPieces(color).boards[PieceTypes::KING].ls1b();
    return kingPosition; //TODO!!
}
