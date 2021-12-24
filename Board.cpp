//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include <memory>
#include <cstring>
#include <regex>
#include "Board.h"
#include "BoardAnalysis.h"


Board::Board () : letterbox{}, pieces{PieceSet{WHITE}, PieceSet{BLACK}}, history{} {
    initializeLetterbox();

    history = std::make_unique<BoardStateHistory>();
}

std::ostream& operator<< (std::ostream& os, const Board& board) {
    std::string base{"................................................................"};
    board.pieces[WHITE].burn(base);
    board.pieces[BLACK].burn(base);



    if (DEBUG) {
        std::stringstream bitboard;

        for (int y = 7; y >= 0; --y) {
            for (int x = 0; x < 8; ++x) {
                bitboard << base[8 * y + x] << " ";
            }
            bitboard << std::endl;
        }
    //
//        os << "letterbox: " << std::endl;

        std::stringstream letterbox;

        for (int y = 7; y >= 0; --y) {
            for (int x = 0; x < 8; ++x) {
                letterbox << *board.letterbox[8 * y + x] << " ";
            }
            letterbox << std::endl;
        }

        if (bitboard.str() != letterbox.str()) {
            std::cerr << "CRITICAL: letterbox and bitboards out of sync!!!" << std::endl;
        }

        os << letterbox.str();

    } else {
        for (int y = 7; y >= 0; --y) {
            for (int x = 0; x < 8; ++x) {
                os << *board.letterbox[8 * y + x] << " ";
            }
            os << std::endl;
        }
    }






    return os;
}

PieceSet Board::getPieces (PieceColor color) const {
    if (color == EMPTY) throw std::runtime_error("No EMPTY in Board::getPieces!!");
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
    int newFiftyMoveReset;
    if (move.isCapture() || move.getMovingPiece(*this).type == PieceTypes::PAWN) {
        newFiftyMoveReset = 0;
    } else {
        newFiftyMoveReset = history->getCurrentFrame().plysSinceFiftyMoveReset + 1;
    }

    BoardState newState{flip(getTurn()), move.raw(), pieceInDestination, newFiftyMoveReset, 0};

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
    for (auto& i: letterbox) i = std::make_unique<Piece>(Pieces::NO_PIECE);

    for (int color = WHITE; color < EMPTY; ++color) {
        for (const PieceType& type : PieceTypes::pieces) {
            auto pieceSet = pieces[color].boards[type];

            for (const Square& square : pieceSet) {
                letterbox[square]->type = type;
                letterbox[square]->color = static_cast<PieceColor>(color);
            }
        }
    }
}

void Board::initializeBitboards () {
    for (const PieceType& type : PieceTypes::pieces) {
        pieces[WHITE].boards[type] = 0;
        pieces[BLACK].boards[type] = 0;
    }

    for (int square = 0; square < 64; ++square) {
        const auto* piece = letterbox[square].get();

        if (*piece != Pieces::NO_PIECE) {
            pieces[piece->color].boards[piece->type] |= Square{square};
        }
    }

    pieces[WHITE].updateOccupancy();
    pieces[BLACK].updateOccupancy();
}

PieceColor Board::getTurn () const {
    return history->getCurrentFrame().turn;
}

const Piece& Board::getPieceAt (Square square) const {
    return *letterbox[square];
}

const PieceSet* Board::getPieces () const {
    return pieces;
}

std::vector<Move> Board::getMoves () const {
//    std::cout << "moi" << std::endl;
    std::vector<Move> moves;

    const Bitboard& checkMask = BoardAnalysis::getCheckMask(*this, getTurn());
    const Bitboard& attackMask = BoardAnalysis::getAttackMask(*this, flip(getTurn()));
    const Bitboard& pinMaskHV = BoardAnalysis::getPinMask<HV>(*this, getTurn());
    const Bitboard& pinMaskD12= BoardAnalysis::getPinMask<D12>(*this, getTurn());

    std::cout << pinMaskHV << std::endl;
//    MoveGeneration::addBishopMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
//    MoveGeneration::addRookMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
    MoveGeneration::addQueenMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
//    MoveGeneration::addKnightMoves(moves, *this, getTurn(), checkMask,  pinMaskHV | pinMaskD12);
//    MoveGeneration::addPawnMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
//    MoveGeneration::addKingMoves(moves, *this, getTurn(), attackMask); // no pinMask as kings cannot be pinned

    return moves;
}


void split (std::string const& string, const char delimiter, std::vector<std::string>& out)
{
    size_t start;
    size_t end = 0;

    while ((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = string.find(delimiter, start);
        out.push_back(string.substr(start, end - start));
    }
}

Board Board::fromFEN (std::string FEN) {
    Board board;

    std::vector<std::string> parts;
    split(FEN, ' ', parts);

    const auto position = std::regex_replace(parts[0], std::regex("\\/"), "");
    std::cout << position.size() << std::endl;

    int square = 0;

    for (auto & i : board.letterbox) i = std::make_unique<Piece>(Pieces::NO_PIECE);

    for (char piece : position) {
        if (std::isdigit(piece)) {
            square += piece - '0';
        } else {
            board.letterbox[square ^ 56] = std::make_unique<Piece>(Pieces::parsePiece(piece));
            square += 1;
        }

    }

    PieceColor turn = parts[1].at(0) == 'w' ? WHITE : BLACK;
    int plysSinceFiftyMoveReset = std::stoi(parts[4]);
    int fullMoveCount = std::stoi(parts[5]);


    BoardState newState = board.history->getCurrentFrame();
    newState.turn = turn;
    newState.plysSinceFiftyMoveReset = plysSinceFiftyMoveReset;
    newState.fullMoveCount = fullMoveCount;
    board.history->popFrame();
    board.history->pushState(newState);

//    std::cout << ranks[1] << std::endl;




//    board.letterbox[3] = std::make_unique<Piece>(PieceTypes::PAWN, WHITE);
//    board.letterbox[7] = std::make_unique<Piece>(PieceTypes::PAWN, BLACK);
//    board.letterbox[43] = std::make_unique<Piece>(PieceTypes::KNIGHT, BLACK);

    board.initializeBitboards();

    return board;
}

Bitboard Board::getOccupancy () const {
    return pieces[WHITE].all | pieces[BLACK].all;
}

Bitboard Board::getBlockers (PieceColor color) const {
    return pieces[color].all;
}

