cmake_minimum_required(VERSION 3.20)
project(shakki4)

set(CMAKE_CXX_STANDARD 20)

add_executable(shakki4 main.cpp PieceColor.cpp PieceColor.h Board.cpp Board.h Square.h PieceSet.cpp PieceSet.h BitboardOperations.h SlidingPieceRays.cpp SlidingPieceRays.h BitboardOperations.cpp mytypes.h)
