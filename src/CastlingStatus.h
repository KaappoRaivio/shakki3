//
// Created by kaappo on 27.12.2021.
//

#pragma once

#include <bitset>
#include "mytypes.h"

typedef int CastlingSide;
struct CastlingStatus {
    std::bitset<4> status;

    explicit CastlingStatus (const std::string& FENCastlingStatus);
    CastlingStatus ();

    friend std::ostream& operator<< (std::ostream& os, const CastlingStatus& status);

    bool canCastle (PieceColor color, CastlingSide side) const;
    void setCanCastle (PieceColor color, CastlingSide side, bool canCastle);
};



