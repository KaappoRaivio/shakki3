//
// Created by kaappo on 1/21/23.
//

#include "UndoException.h"

const char *UndoException::what() const noexcept {
    return "Undo!";
}
