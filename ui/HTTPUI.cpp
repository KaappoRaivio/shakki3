//
// Created by kaappo on 1/15/23.
//

#include "HTTPUI.h"

HTTPUI::HTTPUI () : listenerThread{}, server{} {
    server.Get("/board", [this] (const httplib::Request&, httplib::Response& response) {
        response.set_content(currentBoard.toFEN(), "text/plain");
    });

    server.Post("/makeMove", [this] (const httplib::Request& request, httplib::Response& response) {
        auto search = request.params.find("move");
        if (search == request.params.end()) {
            response.status = 400;
            return;
        }
        const std::string& moveString = search->second;
        std::cout << "Got move:" << moveString << "!" << std::endl;

        try {
            std::vector<Move> legalMoves = currentBoard.getMoves();
            Move move = Move::fromString(moveString, currentBoard);
            bool isValidMove = std::find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end();
            if (isValidMove) {
                Board copy = currentBoard;
                copy.executeMove(move);
                response.body = copy.toFEN();
                response.status = 201;

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

    listenerThread = std::thread{[this] () {
        server.listen("0.0.0.0", 8081);
    }};
}
void HTTPUI::updateValues (Board board) {
    currentBoard = board;
}
Move HTTPUI::getMove () {
    std::unique_lock<std::mutex> lock{cv_m};
    std::cout << "Waiting for UI move! " << std::endl;
    cv.wait(lock, [this] { return incomingMove.has_value(); });
    Move move{incomingMove.value()};
    std::cout << "Got move from UI: " << move << std::endl;

    incomingMove = std::nullopt;
    return move;
}
