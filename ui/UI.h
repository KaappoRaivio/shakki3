//
// Created by kaappo on 8.1.2022.
//

#pragma once


#include "../Board.h"

class UI {
public:
    virtual void updateValues (Board board) = 0;
    virtual Move getMove () = 0;
};

class TtyUI : public UI {
public:
    explicit TtyUI (Board  board);

    void updateValues(Board board) override;
    Move getMove () override;
private:
    Board board;
};


