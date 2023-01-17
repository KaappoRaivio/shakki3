//
// Created by kaappo on 1/15/23.
//

#include "HTTPUI.h"

bool isInteger(const std::string &s) {
    return std::regex_match(s, std::regex("[(-|+)|][0-9]+"));
}

HTTPUI::HTTPUI(AIPlayer *player) : player{player}, listenerThread{}, server{} {
    server.set_default_headers(httplib::Headers{
            {"Access-Control-Allow-Origin",  "http://localhost:8080"},
            {"Access-Control-Allow-Methods", "POST, GET, PUT, OPTIONS"}
    });

//    server.Options("/(.*)",
//                   [&] (const httplib::Request&, httplib::Response& res) {
//                       res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
//                       res.set_header("Content-Type", "text/html; charset=utf-8");
//                       res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept");
//                       res.set_header("Access-Control-Allow-Origin", "*");
//                       res.set_header("Connection", "close");
//                   });

    server.Get("/board", [this](const httplib::Request &, httplib::Response &response) {
        response.set_content(currentBoard.toFEN(), "text/plain");
    });

    server.Post("/makeMove", [this](const httplib::Request &request, httplib::Response &response) {
        auto search = request.params.find("move");
        if (search == request.params.end()) {
            response.status = 400;
            return;
        }
        const std::string &moveString = search->second;

        try {
            std::vector<Move> legalMoves = currentBoard.getMoves();
            Move move = Move::fromString(moveString, currentBoard);
            bool isValidMove = std::find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end();
            if (isValidMove) {
                Board copy = currentBoard;
                copy.executeMove(move);
                response.set_content(copy.toFEN(), "text/plain");
                response.status = 201;
                std::cout << "Got move:" << moveString << "!" << std::endl;

                {
                    std::lock_guard<std::mutex> lock{cv_m};
                    incomingMove = {move};
                }
                cv.notify_all();
            } else {
                response.status = 422;
            }
        } catch (...) {
            response.status = 400;
        }
    });

    // Yes yes I know it should be put but I wasn't able to get CORS working with PUT
    server.Post("/controls/maxTime", [this](const httplib::Request &request, httplib::Response &response) {
        auto search = request.params.find("value");
        if (search != request.params.end()) {
            const std::string &timeString = search->second;
            if (!isInteger(timeString)) {
                response.status = 400;
                return;
            } else {
                std::stringstream ss{timeString};
                int timeMillis;
                ss >> timeMillis;
                std::cout << "New max time set: " << timeMillis << std::endl;

                this->player->allowedTime = std::chrono::milliseconds{timeMillis};
                response.set_content(timeString, "text/plain");
                response.status = 201;
                return;
            }
        }
    });

    server.Get("/controls/maxTime", [this](const httplib::Request &request, httplib::Response &response) {
        std::stringstream ss;
        ss << this->player->allowedTime.count();
        response.set_content(ss.str(), "text/plain");
        response.status = 200;
    });

    server.Get("/controls/useQuiescence", [this](const httplib::Request &request, httplib::Response& response) {
        std::cout << "Getting quiescence " << this->player->getSearch().isUseQuiescenceSearch() << std::endl;
       response.set_content(this->player->getSearch().isUseQuiescenceSearch() ? "true" : "", "text/plain");
       response.status = 200;
    });

    server.Post("/controls/useQuiescence", [this](const httplib::Request &request, httplib::Response& response) {
        auto search = request.params.find("value");
        if (search != request.params.end()) {
            std::string value = search->second;
            if (value.length()) {
                std::cout << "Enabling quiescence " << std::endl;
                this->player->getSearch().setUseQuiescenceSearch(true);
                response.status = 201;
                return;
            } else {
                std::cout << "Disabling quiescence " << std::endl;
                this->player->getSearch().setUseQuiescenceSearch(false);
                response.status = 201;
                return;
            }
        }
    });

    listenerThread = std::thread{[this]() {
        server.listen("0.0.0.0", 8081);
    }};
}

void HTTPUI::updateValues(Board board) {
    currentBoard = board;
}

Move HTTPUI::getMove() {
    std::unique_lock<std::mutex> lock{cv_m};
    std::cout << "Waiting for UI move! " << std::endl;
    cv.wait(lock, [this] { return incomingMove.has_value(); });
    Move move{incomingMove.value()};
    std::cout << "Got move from UI: " << move << std::endl;

    incomingMove = std::nullopt;
    return move;
}
