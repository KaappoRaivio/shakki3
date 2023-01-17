//
// Created by kaappo on 8.1.2022.
//

#pragma once


#include <vector>
#include "Player.h"
#include "ui/UI.h"

class Runner {
public:
    Runner (const std::vector<Player*>& players, Board position, UI* ui);

    PieceColor play ();
private:
    std::vector<Player*> players;
    UI* ui;
    Board position;
};



