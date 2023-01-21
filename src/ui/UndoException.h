//
// Created by kaappo on 1/21/23.
//

#pragma once


#include <exception>

class UndoException : public std::exception {
public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};
