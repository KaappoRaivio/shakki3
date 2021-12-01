cmake_minimum_required(VERSION 3.20)
project(shakki4)

set(CMAKE_CXX_STANDARD 20)

add_executable(shakki4 main.cpp Board.cpp Board.h PieceSet.cpp PieceSet.h BitboardOperations.h SlidingPieceRays.cpp SlidingPieceRays.h BitboardOperations.cpp mytypes.h KnightAttacks.cpp KnightAttacks.h KingAttacks.cpp KingAttacks.h Bitboard.cpp Bitboard.h Move.cpp Move.h Square.cpp Square.h mytypes.cpp BoardUtils.cpp BoardUtils.h)
