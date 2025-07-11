//
// Created by kaappo on 1/21/23.
//

#pragma once


#include <exception>

class UndoException : public std::exception {
public:
    const char *what() const _NOEXCEPT override;
};
