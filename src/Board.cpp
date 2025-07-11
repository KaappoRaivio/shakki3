//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include <memory>
#include <cstring>
#include <regex>
#include <algorithm>
#include "Board.h"
#include "BoardAnalysis.h"
#include "ZobristHash.h"


Board::Board () : letterbox{}, pieces{PieceSet{WHITE}, PieceSet{BLACK}}, history{} {
    initializeLetterbox();
    initializeCurrentHash();
}

std::ostream& operator<< (std::ostream& os, const Board& board) {
    std::string base{"................................................................"};
    board.pieces[WHITE].burn(base);
    board.pieces[BLACK].burn(base);


    if (DEBUG) {
        std::stringstream bitboard;

        for (int y = 7 ; y >= 0 ; --y) {
            for (int x = 0 ; x < 8 ; ++x) {
                bitboard << base[8 * y + x] << " ";
            }
            bitboard << std::endl;
        }

//        os << bitboard.rdbuf();
        //
//        os << "letterbox: " << std::endl;

        std::stringstream letterbox;

        for (int y = 7 ; y >= 0 ; --y) {
            for (int x = 0 ; x < 8 ; ++x) {
                letterbox << board.letterbox[8 * y + x] << " ";
            }
            letterbox << std::endl;
        }

        if (bitboard.str() != letterbox.str()) {
            std::cerr << "CRITICAL: letterbox and bitboards out of sync!!!" << std::endl;
        }

        os << letterbox.str();

    } else {
        for (int y = 7 ; y >= 0 ; --y) {
            for (int x = 0 ; x < 8 ; ++x) {
                os << board.letterbox[8 * y + x] << " ";
            }
            os << std::endl;
        }

        std::string FEN = board.toFEN();
        std::replace(FEN.begin(), FEN.end(), ' ', '_');

        os << std::endl << "https://lichess.org/editor/" << FEN << "?" << (board.getTurn() == WHITE ? "color=white" : "color=black");
    }


    return os;
}

PieceSet Board::getPieces (PieceColor color) const {
    if (color == EMPTY) throw std::runtime_error("No EMPTY in Board::getPieces!!");
    return pieces[color];
}

PieceColor Board::getColorAt (const Square& square) const {
    return letterbox[square].color;
}

const PieceSet& Board::getWhite () const {
    return pieces[WHITE];
}

const PieceSet& Board::getBlack () const {
    return pieces[BLACK];
}

bool Board::is (PieceType type, const Square& at) const {
    PieceColor color = getColorAt(at);
    return bool(pieces[color].boards[type] & Bitboard{at});
}

void Board::setSquare (const Square& where, const Piece& what) {
    const Piece currentPiece = getPieceAt(where);

    letterbox[where] = what;

    if (currentPiece.type != PieceTypes::NO_PIECE) {
        Bitboard& currentPieceBitboard = pieces[currentPiece.color].boards[currentPiece.type];
        currentPieceBitboard ^= where;

        if (currentPiece.type != PieceTypes::KING) {
            pieces[currentPiece.color].all ^= where;
        }

        history.setCurrentFrame().currentHash ^= ZobristHash::getInstance().getHashFor(currentPiece, where);
    }

    if (what.type != PieceTypes::NO_PIECE) {
        pieces[what.color].boards[what.type] |= where;
        if (what.type != PieceTypes::KING) {
            pieces[what.color].all |= where;
        }

        history.setCurrentFrame().currentHash ^= ZobristHash::getInstance().getHashFor(what, where);
    }
}

Piece Board::movePiece (const Square& from, const Square& to) {
    const Piece possiblyCapturedPiece = letterbox[to];
    const Piece movingPiece = letterbox[from];


    setSquare(to, movingPiece);
    setSquare(from, Pieces::NO_PIECE);

    return possiblyCapturedPiece;
}

void Board::unmovePiece (const Piece& capturedPiece, const Square& from, const Square& to) {
    const Piece movingPiece = letterbox[to];

    setSquare(from, movingPiece);
    setSquare(to, capturedPiece);
}

bool Board::isMoveLegal(const Move& move) const {
    auto moves = getMoves(false);
//    return std::find(moves.begin(), moves.end(), move) != moves.end();
    return std::find_if(moves.begin(), moves.end(), [&](const Move& other) {
        return (other.raw() & ~MoveBitmasks::CAPTURE) == (move.raw() & ~MoveBitmasks::CAPTURE);
    }) != moves.end();
}

void Board::executeMove (const Move& move) {
#if DEBUG
    if (not isMoveLegal(move)) {
        throw IllegalMoveException{move};
    }
#endif
    const Piece movingPiece = letterbox[move.getOrigin()];

    if (DEBUG) {
//        std::cout << "debug enabled" << std::endl;
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
    if (move.isCastling(Specials::KING_CASTLE) || move.isCastling(Specials::QUEEN_CASTLE)) {
        moveCastling(move);
    } else if (move.isPromotion()) {
        possiblyCapturedPiece = movePromotion(move);
    } else if (move.isEnPassant()) {
        possiblyCapturedPiece = moveEnPassant(move);
    } else {
        possiblyCapturedPiece = movePiece(move.getOrigin(), move.getDestination());
    }


    int newFiftyMoveReset;
    const BoardState& currentState = history.getCurrentFrame();
    if (move.isCapture() || move.getMovingPiece(*this).type == PieceTypes::PAWN) {
        newFiftyMoveReset = 0;
    } else {
        newFiftyMoveReset = currentState.plysSinceFiftyMoveReset + 1;
    }

    int oldFullmoveCount = currentState.fullMoveCount;
    const CastlingStatus& oldCastlingStatus = currentState.castlingStatus;
    BoardState newState{flip(getTurn()), move.raw(), possiblyCapturedPiece, newFiftyMoveReset, oldFullmoveCount + getTurn(), move.getNewCastlingStatus(*this, oldCastlingStatus, possiblyCapturedPiece), currentState.currentHash};

    newState.currentHash ^= getTurn();
    newState.currentHash ^= flip(getTurn());

    newState.currentHash ^= ZobristHash::getInstance().getHashFor(currentState.castlingStatus);
    newState.currentHash ^= ZobristHash::getInstance().getHashFor(newState.castlingStatus);

    history.pushState(newState);
}

void Board::moveCastling (const Move& move) {
    PieceColor turn = getTurn();
    Square kingPosition{turn == WHITE ? e1 : e8};
    Square rookPosition{turn == WHITE ? (move.isCastling(Specials::KING_CASTLE) ? h1 : a1) : (move.isCastling(Specials::KING_CASTLE) ? h8 : a8)};
    Square newKingPosition{turn == WHITE ? (move.isCastling(Specials::KING_CASTLE) ? g1 : c1) : (move.isCastling(Specials::KING_CASTLE) ? g8 : c8)};
    Square newRookPosition{turn == WHITE ? (move.isCastling(Specials::KING_CASTLE) ? f1 : d1) : (move.isCastling(Specials::KING_CASTLE) ? f8 : d8)};

    movePiece(kingPosition, newKingPosition);
    movePiece(rookPosition, newRookPosition);
}

void Board::unmoveCastling (const Move& move) {
    PieceColor oldTurn = getTurn();
    Square oldKingPosition{oldTurn == WHITE ? e1 : e8};
    Square oldRookPosition{oldTurn == WHITE ? (move.isCastling(Specials::KING_CASTLE) ? h1 : a1) : (move.isCastling(Specials::KING_CASTLE) ? h8 : a8)};

    Square kingPosition{oldTurn == WHITE ? (move.isCastling(Specials::KING_CASTLE) ? g1 : c1) : (move.isCastling(Specials::KING_CASTLE) ? g8 : c8)};
    Square rookPosition{oldTurn == WHITE ? (move.isCastling(Specials::KING_CASTLE) ? f1 : d1) : (move.isCastling(Specials::KING_CASTLE) ? f8 : d8)};

    unmovePiece(Pieces::NO_PIECE, oldKingPosition, kingPosition);
    unmovePiece(Pieces::NO_PIECE, oldRookPosition, rookPosition);
}


void Board::unmakeMove () {
    const BoardState& currentState = history.popFrame();
    const Move& moveToUnmake = currentState.previousMove;
    const Piece& capturedPiece = currentState.capturedPiece;

    if (moveToUnmake.isCastling(Specials::KING_CASTLE) || moveToUnmake.isCastling(Specials::QUEEN_CASTLE)) {
        unmoveCastling(moveToUnmake);
    } else if (moveToUnmake.isPromotion()) {
        unmovePromotion(capturedPiece, moveToUnmake);
    } else if (moveToUnmake.isEnPassant()) {
        unmoveEnPassant(capturedPiece, moveToUnmake);
    } else {
        unmovePiece(capturedPiece, moveToUnmake.getOrigin(), moveToUnmake.getDestination());
    }

}

void Board::initializeLetterbox () {
    for (auto& i : letterbox) {
        i = Pieces::NO_PIECE;
    };

    for (int color = WHITE ; color < EMPTY ; ++color) {
        for (const PieceType& type : PieceTypes::pieces) {
            auto pieceSet = pieces[color].boards[type];

            for (const Square& square : pieceSet) {
                letterbox[square].type = type;
                letterbox[square].color = static_cast<PieceColor>(color);
            }
        }
    }
}

void Board::initializeBitboards () {
    for (const PieceType& type : PieceTypes::pieces) {
        pieces[WHITE].boards[type] = 0;
        pieces[BLACK].boards[type] = 0;
    }

    for (int square = 0 ; square < 64 ; ++square) {
        const auto piece = letterbox[square];

        if (piece != Pieces::NO_PIECE) {
            pieces[piece.color].boards[piece.type] |= Square{square};
        }
    }

    pieces[WHITE].updateOccupancy();
    pieces[BLACK].updateOccupancy();
}

void Board::initializeCurrentHash () {
    uint64_t currentHash = 0;
    for (const Square& square : Bitboard{(uint64_t) -1}) {
        currentHash ^= ZobristHash::getInstance().getHashFor(letterbox[square], square);
    }

    const CastlingStatus& castlingStatus = history.getCurrentFrame().castlingStatus;
    currentHash ^= ZobristHash::getInstance().getHashFor(castlingStatus);
    currentHash ^= ZobristHash::getInstance().getHashFor(getTurn());

    auto& currentFrame = history.setCurrentFrame();
    currentFrame.currentHash = currentHash;
}


PieceColor Board::getTurn () const {
    return history.getCurrentFrame().turn;
}

const Piece& Board::getPieceAt (const Square& square) const {
    if (square.isInvalid()) return Pieces::NO_PIECE;
    return letterbox[square];
}

const PieceSet* Board::getPieces () const {
    return pieces;
}

MOVES Board::getMoves (bool onlyNonQuiet) const {
    MOVES moves;
//    moves.reserve(40);

    const Bitboard& checkMask = BoardAnalysis::getCheckMask(*this, getTurn());
    const Bitboard& attackMask = BoardAnalysis::getAttackMask(*this, flip(getTurn()));
    const Bitboard& pinMaskHV = BoardAnalysis::getPinMask<HV>(*this, getTurn());
    const Bitboard& pinMaskD12 = BoardAnalysis::getPinMask<D12>(*this, getTurn());

    MoveGeneration::addBishopMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12, onlyNonQuiet);
    MoveGeneration::addRookMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12, onlyNonQuiet);
    MoveGeneration::addQueenMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12, onlyNonQuiet);
    MoveGeneration::addKnightMoves(moves, *this, getTurn(), checkMask, pinMaskHV | pinMaskD12, onlyNonQuiet); // a combination of the pinmasks as pinned knights can never move
    MoveGeneration::addPawnMoves(moves, *this, getTurn(), checkMask, pinMaskHV, pinMaskD12, onlyNonQuiet);
    MoveGeneration::addKingMoves(moves, *this, getTurn(), attackMask, onlyNonQuiet); // no pinMask as kings cannot be pinned

    return moves;
}


void split (std::string const& string, const char delimiter, std::vector<std::string>& out) {
    size_t start;
    size_t end = 0;

    while ((start = string.find_first_not_of(delimiter, end)) != std::string::npos) {
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

    for (auto& i : board.letterbox) i = Pieces::NO_PIECE;

    for (char piece : position) {
        if (std::isdigit(piece)) {
            square += piece - '0';
        } else {
            board.letterbox[square ^ 56] = Pieces::parsePiece(piece);
            square += 1;
        }

    }

    PieceColor turn = parts[1].at(0) == 'w' ? WHITE : BLACK;
    CastlingStatus status{parts[2]};


    int plysSinceFiftyMoveReset = std::stoi(parts[4]);
    int fullMoveCount = std::stoi(parts[5]);



//    std::cout << ranks[1] << std::endl;




//    board.letterbox[3] = std::make_unique<Piece>(PieceTypes::PAWN, WHITE);
//    board.letterbox[7] = std::make_unique<Piece>(PieceTypes::PAWN, BLACK);
//    board.letterbox[43] = std::make_unique<Piece>(PieceTypes::KNIGHT, BLACK);

    board.initializeBitboards();

    Move previousMove = Moves::NO_MOVE;
    if (parts[3].at(0) != '-') {
        const Square& enPassantTargetSquare = Square::fromString(parts[3]);
        switch (enPassantTargetSquare.getY()) {
            case 2:
                previousMove = Move{board, enPassantTargetSquare.move(SOUTH), enPassantTargetSquare.move(NORTH)};
                break;
            case 5:
                previousMove = Move{board, enPassantTargetSquare.move(NORTH), enPassantTargetSquare.move(SOUTH)};
                break;
            default:
                throw std::runtime_error("Invalid en passant target square!");
        }
    }

    BoardState newState = board.history.getCurrentFrame();
    newState.turn = turn;
    newState.plysSinceFiftyMoveReset = plysSinceFiftyMoveReset;
    newState.fullMoveCount = fullMoveCount;
    newState.castlingStatus = status;
    newState.previousMove = previousMove.raw() | Specials::DOUBLE_PAWN_PUSH;
    board.history.popFrame();
    board.history.pushState(newState);


    board.initializeCurrentHash();
    return board;
}

Bitboard Board::getOccupancy (bool includeKings) const {
    Bitboard occupancy = pieces[WHITE].all | pieces[BLACK].all;
    if (includeKings) {
        occupancy |= pieces[WHITE].boards[PieceTypes::KING];
        occupancy |= pieces[BLACK].boards[PieceTypes::KING];
    } else {
        occupancy |= pieces[flip(getTurn())].boards[PieceTypes::KING];
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

const BoardStateHistory& Board::getHistory () const {
    return history;
}

std::string Board::toFEN () const {
    std::stringstream out;
    out << toFENShort();

    out << " " << history.getCurrentFrame().plysSinceFiftyMoveReset;
    out << " " << history.getCurrentFrame().fullMoveCount;

    return out.str();
}

std::string Board::toFENShort () const {
    std::stringstream out;

    for (int y = 7 ; y >= 0 ; --y) {
        for (int x = 0 ; x < 8 ; ++x) {
            if (letterbox[8 * y + x].type == PieceTypes::NO_PIECE) {
                int emptyCount = 0;
                while (x < 8 && letterbox[8 * y + x].type == PieceTypes::NO_PIECE) {
                    ++x;
                    ++emptyCount;
                }
                out << emptyCount;
                --x;
                continue;
            }

            out << letterbox[8 * y + x];
        }
        if (y != 0)
            out << "/";
    }

    out << " " << (getTurn() == WHITE ? 'w' : 'b');
    out << " " << history.getCurrentFrame().castlingStatus;

    const Move& previousMove = Move{history.getCurrentFrame().previousMove};
    if (previousMove.isDoublePawnPush()) {
        const Square& square = previousMove.getOrigin().move(NORTH, flip(getTurn()));
        std::stringstream ss;
        ss << square;
        std::string str = ss.str();
        std::transform(str.begin(), str.end(), str.begin(),
                       [] (unsigned char c) { return tolower(c); });

        out << " " << str;
    } else {
        out << " -";
    }
    return out.str();
}

Piece Board::movePromotion (const Move& promotionMove) {
    auto possiblyCapturedPiece = movePiece(promotionMove.getOrigin(), promotionMove.getDestination());
    setSquare(promotionMove.getDestination(), Pieces::pieces[promotionMove.getPromotedPiece()][promotionMove.getMovingPiece(*this).color]);

    return possiblyCapturedPiece;
}

void Board::unmovePromotion (const Piece& capturedPiece, const Move& move) {
    unmovePiece(capturedPiece, move.getOrigin(), move.getDestination());

    setSquare(move.getOrigin(), Pieces::pieces[PieceTypes::PAWN][getPieceAt(move.getOrigin()).color]);
}

Piece Board::moveEnPassant (const Move& move) {
    const Square& captureSquare = Square{move.getOrigin().getY(), move.getDestination().getX()};
    Piece capturedPiece = letterbox[captureSquare];
    movePiece(move.getOrigin(), move.getDestination());
    setSquare(captureSquare, Pieces::NO_PIECE);
    return capturedPiece;
}

void Board::unmoveEnPassant (const Piece& capturedPiece, const Move& moveToUnmake) {
    const Square& captureSquare = Square{moveToUnmake.getOrigin().getY(), moveToUnmake.getDestination().getX()};
    unmovePiece(Pieces::NO_PIECE, moveToUnmake.getOrigin(), moveToUnmake.getDestination());
    setSquare(captureSquare, capturedPiece);
}

bool Board::isEnPassantPossible () const {
    return Move{history.getCurrentFrame().previousMove}.isDoublePawnPush();
}

bool Board::isCheck () const {
    return BoardAnalysis::isCheck(*this);
}

bool Board::isCheckmate () const {
    return getMoves().empty() && isCheck();
}


uint64_t Board::hash () const {
    return history.getCurrentFrame().currentHash;
}

void Board::executeSequenceOfMoves (const std::vector<std::string>& moves) {
    for (const std::string& moveString : moves) {
        executeMove(Move::fromString(moveString, *this));
    }
}



namespace std {
    template <>
    struct hash<Board>
    {
        std::size_t operator () (const Board& k) const {
            return k.hash();
        }
    };

}

const char *IllegalMoveException::what() const noexcept {
    std::stringstream ss;
    ss << "Move " << move << " is illegal!";
    return (new std::string{ss.str()})->c_str();
}
