//
// Created by kaappo on 10.12.2021.
//

#include "StringUtils.h"

std::string StringUtils::burnString (std::string base, const std::string& stringToBurn) {
    auto minLength = std::min(base.size(), stringToBurn.size());

    for (size_t i = 0; i < minLength; ++i) {
        auto character = stringToBurn.at(i);

        switch (character) {
            case '.':
            case ' ':
            case '\n':
                break;
            default:
                switch (base[i]) {
                    case '.':
                    case ' ':
                    case '\n':
                        base[i] = character;
                        break;
                    default:
                        base[i] = '!';
                        break;
                }

        }
    }

    return base;
}
