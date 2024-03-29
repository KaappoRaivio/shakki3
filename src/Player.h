//
// Created by kaappo on 8.1.2022.
//

#pragma once


#include <chrono>
#include "Board.h"
#include "ui/UI.h"
#include "ai/Search.h"

class Player {
public:
    virtual ~Player() = default;

    virtual Move getMove(Board board) = 0;

    virtual bool isHumanPlayer() = 0;
};


class HumanPlayer : public Player {
private:
    UI *ui;

public:
    ~HumanPlayer() override = default;

    explicit HumanPlayer(UI *ui);

    Move getMove(Board board) override;

    bool isHumanPlayer() override;
};

class AIPlayer : public Player {
private:
    Search search;
public:
    ~AIPlayer() override = default;

    bool isHumanPlayer() override;

    AIPlayer(int searchDepth, std::chrono::milliseconds allowedTime)
            : search{}, allowedTime{allowedTime}, searchDepth{searchDepth} {}


    Move getMove(Board board) override;

    std::chrono::milliseconds allowedTime = std::chrono::seconds{10};
    int searchDepth;

    Search &getSearch();
};