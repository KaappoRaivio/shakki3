//
// Created by kaappo on 8.1.2022.
//

#pragma once


#include "Board.h"
#include "ui/UI.h"
#include "ai/Search.h"

class Player {
public:
virtual ~Player() = default;
    virtual Move getMove (Board board) = 0;
};


class HumanPlayer : public Player {
private:
    UI* ui;

public:
~HumanPlayer() override = default;
    explicit HumanPlayer (UI* ui);

    Move getMove (Board board) override;
};

class AIPlayer : public Player {
private:
    Search search;
public:
~AIPlayer() override = default;

    explicit AIPlayer (int searchDepth);

    Move getMove (Board board) override;

    int searchDepth;
};