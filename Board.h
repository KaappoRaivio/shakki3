//
// Created by kaappo on 20.11.2021.
//


#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>
#include <memory>
#include "PieceSet.h"
#include "Move.h"
#include "BoardUtils.h"

class Move;
class BoardStateHistory;

class Board {
public:
    static Board fromFEN (std::string FEN);
    std::string toFEN () const;
    std::string toFENShort () const;

    Board ();

    PieceSet getPieces (PieceColor color) const;
    friend std::ostream& operator<< (std::ostream& os, const Board& board);

    PieceColor getColorAt (const Square& square) const;
    bool is(PieceType type, const Square& at) const;

    void executeMove (const Move& move);
    void unmakeMove ();

    PieceColor getTurn () const;

    std::vector<Move> getMoves () const;

    const Piece& getPieceAt (const Square& square) const;
    const PieceSet* getPieces () const;

    Bitboard getOccupancy (bool includeKings) const;
    Bitboard getBlockers (PieceColor color, bool includeKing) const;

    const BoardStateHistory& getHistory () const;


    bool isEnPassantPossible () const;


private:
    Piece letterbox[8 * 8];
    PieceSet pieces[2];
    BoardStateHistory history;

    void initializeLetterbox ();

    const PieceSet& getWhite () const;
    const PieceSet& getBlack () const;

    void initializeBitboards ();

    Piece movePiece (const Square& from, const Square& to);

    void moveCastling (const Move& move);

    void unmovePiece (const Piece& capturedPiece, const Square& from, const Square& to);

    void unmoveCastling (const Move& move);

    Piece movePromotion (const Move& promotionMove);

    void setSquare (const Square& where, const Piece& what);

    void unmovePromotion (const Piece& capturedPiece, const Move move);

    Piece moveEnPassant (const Move& move);

    void unmoveEnPassant (const Piece& capturedPiece, const Move& moveToUnmake);

};


