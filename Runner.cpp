//
// Created by kaappo on 8.1.2022.
//

#include "Runner.h"

#include <utility>
#include <chrono>

Runner::Runner (const std::vector<Player*>& players, Board position, UI* ui) : players{players}, ui{ui}, position{std::move(position)} {
}

PieceColor Runner::play () {
    while (true) {
        Player* currentPlayer = players[position.getTurn()];

        ui->updateValues(position);
        if (position.isCheckmate()) {
            std::cout << "WINNER: " << flip(position.getTurn()) << std::endl;
//            std::cout << position << std::endl;
            return flip(position.getTurn());
        }


        auto startTime = std::chrono::system_clock::now();
        Move move = currentPlayer->getMove(position);

        auto endTime = std::chrono::system_clock::now();
        std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms!" << std::endl;

        position.executeMove(move);
    }
}
