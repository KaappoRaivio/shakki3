//
// Created by kaappo on 16.12.2021.
//

#include "MyMath.h"
#include <cmath>
#include <iostream>

namespace MyMath {
    int modulus (int a, int b) {
        return ((a % b) + b) % b;
    }
}
