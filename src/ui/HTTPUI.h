//
// Created by kaappo on 1/15/23.
//

#pragma once

#include <optional>
#include "httplib/httplib.h"
#include "UI.h"
#include "../Player.h"


class HTTPUI : public UI {
    AIPlayer* player;
    std::thread listenerThread;
    httplib::Server server;
    Board currentBoard;
    std::optional<Move> incomingMove = std::nullopt;
    bool undo = false;

    std::condition_variable cv;
    std::mutex cv_m;
public:
    HTTPUI (AIPlayer* player);
    void updateValues (Board board) override;
    Move getMove () override;
};
