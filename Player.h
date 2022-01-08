//
// Created by kaappo on 8.1.2022.
//

#pragma once


#include "Board.h"
#include "UI.h"
#include "ai/Search.h"

class Player {
public:
    virtual Move getMove (Board board) = 0;
};


class HumanPlayer : public Player {
private:
    UI* ui;

public:
    explicit HumanPlayer (UI* ui);

    Move getMove (Board board) override;
};

class AIPlayer : public Player {
private:
    Search search;
public:
    AIPlayer ();

    Move getMove (Board board) override;
};