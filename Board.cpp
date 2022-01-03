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

//        os << bitboard.rdbuf();
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

void Board::setSquare (const Square& where, const Piece& what) {
    const Piece currentPiece = getPieceAt(where);

    letterbox[where]->type = what.type;
    letterbox[where]->color = what.color;

    if (currentPiece.type != PieceTypes::NO_PIECE) {
        Bitboard& currentPieceBitboard = pieces[currentPiece.color].boards[currentPiece.type];
        currentPieceBitboard ^= where;

        if (currentPiece.type != PieceTypes::KING) {
            pieces[currentPiece.color].all ^= where;
        }
    }

    if (what.type != PieceTypes::NO_PIECE) {
        pieces[what.color].boards[what.type] |= where;
        if (what.type != PieceTypes::KING) {
            pieces[what.color].all |= where;
        }
    }
}

Piece Board::movePiece (const Square& from, const Square& to) {
    const Piece possiblyCapturedPiece = *letterbox[to];
    const Piece movingPiece = *letterbox[from];


    setSquare(to, movingPiece);
    setSquare(from, Pieces::NO_PIECE);

    return possiblyCapturedPiece;
}

void Board::unmovePiece (const Piece& capturedPiece, const Square& from, const Square& to) {
    const Piece movingPiece = *letterbox[to];

    setSquare(from, movingPiece);
    setSquare(to, capturedPiece);
}

void Board::executeMove (const Move& move) {
    const Piece movingPiece = *letterbox[move.getOrigin()];

    if (DEBUG) {
        if (movingPiece.color == EMPTY) {
            std::stringstream ss;
            ss << "Origin square for move " << move << " is empty! \n" << *this;
            throw std::runtime_error(ss.str());
        }
        if (movingPiece.color != getTurn()) {
            throw std::runtime_error("Wrong turn!");
        }
    }


    Piece possiblyCapturedPiece{PieceTypes::NO_PIECE, EMPTY};
    if (move.isCastling(MoveBitmasks::KING_CASTLE) || move.isCastling(MoveBitmasks::QUEEN_CASTLE)) {
        moveCastling(move);
    } else if (move.isPromotion()) {
        possiblyCapturedPiece = movePromotion(move);
    } else {
        possiblyCapturedPiece = movePiece(move.getOrigin(), move.getDestination());
    }


    int newFiftyMoveReset;
    if (move.isCapture() || move.getMovingPiece(*this).type == PieceTypes::PAWN) {
        newFiftyMoveReset = 0;
    } else {
        newFiftyMoveReset = history->getCurrentFrame().plysSinceFiftyMoveReset + 1;
    }

    int oldFullmoveCount = history->getCurrentFrame().fullMoveCount;
    const CastlingStatus& oldCastlingStatus = history->getCurrentFrame().castlingStatus;
    BoardState newState{flip(getTurn()), move.raw(), possiblyCapturedPiece, newFiftyMoveReset, oldFullmoveCount + 1, move.getNewCastlingStatus(*this, oldCastlingStatus)};
    history->pushState(newState);
}

void Board::moveCastling(const Move& move) {
    PieceColor turn = getTurn();
    Square kingPosition{turn == WHITE ? e1 : e8};
    Square rookPosition{turn == WHITE ? (move.isCastling(MoveBitmasks::KING_CASTLE) ? h1 : a1) : (move.isCastling(MoveBitmasks::KING_CASTLE) ? h8 : a8)};
    Square newKingPosition{turn == WHITE ? (move.isCastling(MoveBitmasks::KING_CASTLE) ? g1 : c1) : (move.isCastling(MoveBitmasks::KING_CASTLE) ? g8 : c8)};
    Square newRookPosition{turn == WHITE ? (move.isCastling(MoveBitmasks::KING_CASTLE) ? f1 : d1) : (move.isCastling(MoveBitmasks::KING_CASTLE) ? f8 : d8)};

    movePiece(kingPosition, newKingPosition);
    movePiece(rookPosition, newRookPosition);
}

void Board::unmoveCastling (const Move& move) {
    PieceColor oldTurn = getTurn();
    Square oldKingPosition{oldTurn == WHITE ? e1 : e8};
    Square oldRookPosition{oldTurn == WHITE ? (move.isCastling(MoveBitmasks::KING_CASTLE) ? h1 : a1) : (move.isCastling(MoveBitmasks::KING_CASTLE) ? h8 : a8)};

    Square kingPosition{oldTurn == WHITE ? (move.isCastling(MoveBitmasks::KING_CASTLE) ? g1 : c1) : (move.isCastling(MoveBitmasks::KING_CASTLE) ? g8 : c8)};
    Square rookPosition{oldTurn == WHITE ? (move.isCastling(MoveBitmasks::KING_CASTLE) ? f1 : d1) : (move.isCastling(MoveBitmasks::KING_CASTLE) ? f8 : d8)};

    unmovePiece(Pieces::NO_PIECE, oldKingPosition, kingPosition);
    unmovePiece(Pieces::NO_PIECE, oldRookPosition, rookPosition);
}



void Board::unmakeMove () {
    const BoardState& currentState = history->popFrame();
    const Move& moveToUnmake = currentState.previousMove;
    const Piece& capturedPiece = currentState.capturedPiece;

    if (moveToUnmake.isCastling(MoveBitmasks::KING_CASTLE) || moveToUnmake.isCastling(MoveBitmasks::QUEEN_CASTLE)) {
        unmoveCastling(moveToUnmake);
    } else if (moveToUnmake.isPromotion()) {
        unmovePromotion(capturedPiece, moveToUnmake);
    } else {
        unmovePiece(capturedPiece, moveToUnmake.getOrigin(), moveToUnmake.getDestination());
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
    if (square.isInvalid()) return Pieces::NO_PIECE;
    return *letterbox[square];
}

const PieceSet* Board::getPieces () const {
    return pieces;
}

std::vector<Move> Board::getMoves () const {
    std::vector<Move> moves;

    const Bitboard& checkMask  = BoardAnalysis::getCheckMask(*this, getTurn());
    const Bitboard& attackMask = BoardAnalysis::getAttackMask(*this, flip(getTurn()));
    const Bitboard& pinMaskHV  = BoardAnalysis::getPinMask<HV>(*this, getTurn());
    const Bitboard& pinMaskD12 = BoardAnalysis::getPinMask<D12>(*this, getTurn());

    MoveGeneration::addBishopMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
    MoveGeneration::addRookMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
    MoveGeneration::addQueenMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
    MoveGeneration::addKnightMoves(moves, *this, getTurn(), checkMask,  pinMaskHV | pinMaskD12);
    MoveGeneration::addPawnMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12);
    MoveGeneration::addKingMoves(moves, *this, getTurn(), attackMask); // no pinMask as kings cannot be pinned

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
    CastlingStatus status{parts[2]};
    int plysSinceFiftyMoveReset = std::stoi(parts[4]);
    int fullMoveCount = std::stoi(parts[5]);


    BoardState newState = board.history->getCurrentFrame();
    newState.turn = turn;
    newState.plysSinceFiftyMoveReset = plysSinceFiftyMoveReset;
    newState.fullMoveCount = fullMoveCount;
    newState.castlingStatus = status;
    board.history->popFrame();
    board.history->pushState(newState);

//    std::cout << ranks[1] << std::endl;




//    board.letterbox[3] = std::make_unique<Piece>(PieceTypes::PAWN, WHITE);
//    board.letterbox[7] = std::make_unique<Piece>(PieceTypes::PAWN, BLACK);
//    board.letterbox[43] = std::make_unique<Piece>(PieceTypes::KNIGHT, BLACK);

    board.initializeBitboards();

    return board;
}

Bitboard Board::getOccupancy (bool includeKings) const {
    Bitboard occupancy = pieces[WHITE].all | pieces[BLACK].all;
    if (includeKings) {
        occupancy |= pieces[WHITE].boards[PieceTypes::KING];
        occupancy |= pieces[BLACK].boards[PieceTypes::KING];
    }

    return occupancy;
}

Bitboard Board::getBlockers (PieceColor color, bool includeKing) const {
    Bitboard occupancy = pieces[color].all;
    if (includeKing) {
        occupancy |= pieces[color].boards[PieceTypes::KING];
    }

    return occupancy;
}

const BoardStateHistory* Board::getHistory () const {
    return history.get();
}

std::string Board::toFEN () const {
    std::stringstream out;

    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            if (letterbox[8 * y + x]->type == PieceTypes::NO_PIECE) {
                int emptyCount = 0;
                while (x < 8 && letterbox[8 * y + x]->type == PieceTypes::NO_PIECE) {
                    ++x;
                    ++emptyCount;
                }
                out << emptyCount;
                --x;
                continue;
            }

            out << *letterbox[8 * y + x];
        }
        if (y != 0)
            out << "/";
    }

    out << " " << (getTurn() == WHITE ? 'w' : 'b');
    out << " " << history->getCurrentFrame().castlingStatus;
    out << " -";
    out << " " << history->getCurrentFrame().plysSinceFiftyMoveReset;
    out << " " << history->getCurrentFrame().fullMoveCount;

    return out.str();
}

Piece Board::movePromotion (const Move& promotionMove) {
    auto possiblyCapturedPiece = movePiece(promotionMove.getOrigin(), promotionMove.getDestination());
    setSquare(promotionMove.getDestination(), Pieces::pieces[promotionMove.getPromotedPiece()][promotionMove.getMovingPiece(*this).color]);

    return possiblyCapturedPiece;
}

void Board::unmovePromotion (const Piece& capturedPiece, const Move move) {
    unmovePiece(capturedPiece, move.getOrigin(), move.getDestination());

    setSquare(move.getOrigin(), Pieces::pieces[PieceTypes::PAWN][getPieceAt(move.getOrigin()).color]);
}

