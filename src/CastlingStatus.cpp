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
        setCanCastle(WHITE, Specials::KING_CASTLE, true);
    }

    if (FENCastlingStatus.find('Q') != std::string::npos) {
        setCanCastle(WHITE, Specials::QUEEN_CASTLE, true);
    }

    if (FENCastlingStatus.find('k') != std::string::npos) {
        setCanCastle(BLACK, Specials::KING_CASTLE, true);
    }

    if (FENCastlingStatus.find('q') != std::string::npos) {
        setCanCastle(BLACK, Specials::QUEEN_CASTLE, true);
    }
}

CastlingStatus::CastlingStatus () : status{0xF} {}

std::ostream& operator<< (std::ostream& os, const CastlingStatus& status) {
    if (status.status.none()) return os << '-';


    if (status.canCastle(WHITE, Specials::KING_CASTLE)) os << 'K';
    if (status.canCastle(WHITE, Specials::QUEEN_CASTLE)) os << 'Q';
    if (status.canCastle(BLACK, Specials::KING_CASTLE)) os << 'k';
    if (status.canCastle(BLACK, Specials::QUEEN_CASTLE)) os << 'q';

    return os;
}
