//
// Created by kaappo on 1/15/23.
//

#pragma once

#include <optional>
#include "httplib/httplib.h"
#include "UI.h"


class HTTPUI : public UI {
    std::thread listenerThread;
    httplib::Server server;
    Board currentBoard;
    std::optional<Move> incomingMove = std::nullopt;

    std::condition_variable cv;
    std::mutex cv_m;
public:
    HTTPUI ();
    void updateValues (Board board) override;
    Move getMove () override;
};
