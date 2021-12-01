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
    std::unique_ptr<Piece>& movingPiece = letterbox[move.getOrigin()];
    std::unique_ptr<Piece>& pieceInDestination = letterbox[move.getDestination()];

    if (movingPiece->color != getTurn()) {
        throw std::runtime_error("Wrong turn!");
    }

    // letterbox
    letterbox[move.getDestination()]->type = movingPiece->type;
    letterbox[move.getDestination()]->color = movingPiece->color;

    letterbox[move.getOrigin()]->type = PieceTypes::NO_PIECE;
    letterbox[move.getOrigin()]->color = EMPTY;

    //bitboards
    auto originPieceSet = pieces[movingPiece->color].boards[movingPiece->type];
    originPieceSet ^= move.getOrigin();
    originPieceSet ^= move.getDestination();

    if (*pieceInDestination != Pieces::NO_PIECE) {
        auto destinationPieceSet = pieces[pieceInDestination->color].boards[pieceInDestination->type];
        destinationPieceSet ^= move.getDestination();
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

