cmake_minimum_required(VERSION 3.20)
project(shakki4)

set(CMAKE_PREFIX_PATH $CMAKE_PREFIX_PATH $CMAKE_CURRENT_SOURCE_DIR/lib)

set(CMAKE_CXX_STANDARD 23)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra -fconcepts")


add_executable(shakki4 main.cpp Board.cpp Board.h PieceSet.cpp PieceSet.h BitboardOperations.h BitboardOperations.cpp mytypes.h Bitboard.cpp Bitboard.h Move.cpp Move.h Square.cpp Square.h mytypes.cpp BoardUtils.cpp BoardUtils.h Attacks.cpp Attacks.h StringUtils.cpp StringUtils.h MyMath.cpp MyMath.h SlidingPieceAttacks_impl.h BoardAnalysis.cpp BoardAnalysis.h TerminalColors.h InBetween.cpp InBetween.h CastlingStatus.cpp CastlingStatus.h)
add_executable(tests tests/all.cpp Board.cpp Board.h PieceSet.cpp PieceSet.h BitboardOperations.h BitboardOperations.cpp mytypes.h Bitboard.cpp Bitboard.h Move.cpp Move.h Square.cpp Square.h mytypes.cpp BoardUtils.cpp BoardUtils.h Attacks.cpp Attacks.h StringUtils.cpp StringUtils.h MyMath.cpp MyMath.h SlidingPieceAttacks_impl.h BoardAnalysis.cpp BoardAnalysis.h TerminalColors.h InBetween.cpp InBetween.h  CastlingStatus.cpp CastlingStatus.h tests/Bitboard.test.cpp tests/Square.test.cpp tests/BoardAnalysis.test.cpp tests/Board.test.cpp tests/TestHelpers.h tests/TestHelpers.cpp tests/Move.test.cpp)

find_package(Catch2 REQUIRED)
target_link_libraries(tests Catch2::Catch2)

include(Catch)
catch_discover_tests(tests)
enable_testing()
