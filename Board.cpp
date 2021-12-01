//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include <memory>
#include "Board.h"


Board::Board () : pieces{PieceSet{WHITE}, PieceSet{BLACK}}, letterbox{} {
    initializeLetterbox();

    history = std::make_unique<BoardStateHistory>();
}

std::ostream& operator<< (std::ostream& os, const Board& board) {
    std::string base{"................................................................"};
    board.pieces[WHITE].burn(base);
    board.pieces[BLACK].burn(base);

    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            os << base[8 * y + x] << " ";
        }
        os << std::endl;
    }
//
    os << "letterbox: " << std::endl;
    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            os << *board.letterbox[8 * y + x] << " ";
        }
        os << std::endl;
    }





    return os;
}

PieceSet Board::getPieceSet (PieceColor color) const {
    if (color == EMPTY) throw std::runtime_error("No EMPTY in Board::getPieceSet!!");
    return pieces[color];
}

PieceColor Board::getColorAt (Square square) const {
    if (pieces[WHITE].hasPiece(square)) return WHITE;
    else if (pieces[BLACK].hasPiece(square)) return BLACK;
    else return EMPTY;
}

const PieceSet& Board::getWhite () const {
    return pieces[WHITE];
}

const PieceSet& Board::getBlack () const {
    return pieces[BLACK];
}

bool Board::is (PieceType type, Square at) const {
    PieceColor color = getColorAt(at);
    return bool(pieces[color].boards[type] & Bitboard{at});
}

void Board::executeMove (const Move& move) {
//    const std::unique_ptr<Piece>& movingPiece = letterbox[move.getOrigin()];
    const Piece movingPiece = *letterbox[move.getOrigin()];
    const Piece pieceInDestination = *letterbox[move.getDestination()];

    if (movingPiece.color != getTurn()) {
        throw std::runtime_error("Wrong turn!");
    }

    // letterbox
    letterbox[move.getDestination()]->type = movingPiece.type;
    letterbox[move.getDestination()]->color = movingPiece.color;

    letterbox[move.getOrigin()]->type = PieceTypes::NO_PIECE;
    letterbox[move.getOrigin()]->color = PieceColor::EMPTY;

    // bitboards
    Bitboard& allPieces = pieces[movingPiece.color].all;
    Bitboard& movingPieceSet = pieces[movingPiece.color].boards[movingPiece.type];
    movingPieceSet ^= move.getOrigin();
    movingPieceSet ^= move.getDestination();

//    std::cout << movingPieceSet << std::endl;

    allPieces ^= move.getOrigin();
    allPieces ^= move.getDestination();

    if (pieceInDestination != Pieces::NO_PIECE) {
        Bitboard& allOppositePieces = pieces[pieceInDestination.color].all;
        Bitboard& destinationPieceSet = pieces[pieceInDestination.color].boards[pieceInDestination.type];
        destinationPieceSet ^= move.getDestination();
        allOppositePieces ^= move.getDestination();
    }

//    std::cout << move << std::endl;

    // create copy-make frame
//    history->createNewFrame();

    BoardState newState {
        flip(getTurn()),
        move.raw(),
        pieceInDestination,
        history->getCurrentFrame().plysSinceFiftyMoveReset + 1
    };

    history->pushState(newState);
//    std::cout << history->getCurrentFrame().previousMove << std::endl;
}

void Board::unmakeMove () {
    const BoardState& currentState = history->popFrame();
    const Move& moveToUnmake = currentState.previousMove;
    const Piece& capturedPiece = currentState.capturedPiece;

    const Piece movingPiece = *letterbox[moveToUnmake.getDestination()];

    // letterbox
    letterbox[moveToUnmake.getOrigin()]->type = movingPiece.type;
    letterbox[moveToUnmake.getOrigin()]->color = movingPiece.color;

    letterbox[moveToUnmake.getDestination()]->type = capturedPiece.type;
    letterbox[moveToUnmake.getDestination()]->color = capturedPiece.color;

    // bitboards
    Bitboard& movingPieceSet = pieces[movingPiece.color].boards[movingPiece.type];
    movingPieceSet ^= moveToUnmake.getDestination();
    movingPieceSet ^= moveToUnmake.getOrigin();

    if (capturedPiece != Pieces::NO_PIECE) {
        Bitboard& capturedPieceSet = pieces[capturedPiece.color].boards[capturedPiece.type];
        capturedPieceSet ^= moveToUnmake.getDestination();
    }
}

void Board::initializeLetterbox () {
//    std::cout << "resetting" << std::endl;
//    for (auto& i: letterbox) i = &Pieces::NO_PIECE;
    for (auto& i: letterbox) i = std::make_unique<Piece>(Pieces::NO_PIECE);
//    std::cout << "reset" << std::endl;


    for (int color = WHITE; color < EMPTY; ++color) {
//        std::cout << "Color: " << color << std::endl;
        for (const PieceType& type : PieceTypes::pieces) {
//            std::cout << *this << std::endl;
//            std::cout << "\tType: " << type << std::endl;
            auto pieceSet = pieces[color].boards[type];

            for (const Square& square : pieceSet) {
//                std::cout << "\t\t" << square << std::endl;

                letterbox[square]->type = type;
                letterbox[square]->color = static_cast<PieceColor>(color);

//                std::cout << "\t\tSetting" << square << " to " << *letterbox[square] << std::endl;

            }
        }
    }
}

PieceColor Board::getTurn () const {
    return history->getCurrentFrame().turn;
}

