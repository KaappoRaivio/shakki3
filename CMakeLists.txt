cmake_minimum_required(VERSION 3.20)
project(shakki4)

set(CMAKE_CXX_STANDARD 20)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra")

add_executable(shakki4 main.cpp Board.cpp Board.h PieceSet.cpp PieceSet.h BitboardOperations.h BitboardOperations.cpp mytypes.h Bitboard.cpp Bitboard.h Move.cpp Move.h Square.cpp Square.h mytypes.cpp BoardUtils.cpp BoardUtils.h Attacks.cpp Attacks.h StringUtils.cpp StringUtils.h MyMath.cpp MyMath.h SlidingPieceAttacks_impl.h BoardAnalysis.cpp BoardAnalysis.h TerminalColors.h InBetween.cpp InBetween.h)
