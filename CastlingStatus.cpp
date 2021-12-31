//
// Created by kaappo on 27.12.2021.
//

#include "CastlingStatus.h"
#include "mytypes.h"
#include "Move.h"

bool CastlingStatus::canCastle (PieceColor color, CastlingSide side) const {
    return status[2 * color + (side - 2)];
}

void CastlingStatus::setCanCastle (PieceColor color, CastlingSide side, bool canCastle) {
    status[2 * color + (side - 2)] = canCastle;
}

CastlingStatus::CastlingStatus (const std::string& FENCastlingStatus) : status{} {
    if (FENCastlingStatus.find('K') != std::string::npos) {
        setCanCastle(WHITE, MoveBitmasks::KING_CASTLE, true);
    }

    if (FENCastlingStatus.find('Q') != std::string::npos) {
        setCanCastle(WHITE, MoveBitmasks::QUEEN_CASTLE, true);
    }

    if (FENCastlingStatus.find('k') != std::string::npos) {
        setCanCastle(BLACK, MoveBitmasks::KING_CASTLE, true);
    }

    if (FENCastlingStatus.find('q') != std::string::npos) {
        setCanCastle(BLACK, MoveBitmasks::QUEEN_CASTLE, true);
    }
}

CastlingStatus::CastlingStatus () : status{0xF} {}

std::ostream& operator<< (std::ostream& os, const CastlingStatus& status) {
    if (status.status.none()) return os << '-';


    if (status.canCastle(WHITE, MoveBitmasks::KING_CASTLE)) os << 'K';
    if (status.canCastle(WHITE, MoveBitmasks::QUEEN_CASTLE)) os << 'Q';
    if (status.canCastle(BLACK, MoveBitmasks::KING_CASTLE)) os << 'k';
    if (status.canCastle(BLACK, MoveBitmasks::QUEEN_CASTLE)) os << 'q';

    return os;
}
