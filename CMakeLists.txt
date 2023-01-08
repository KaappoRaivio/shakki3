cmake_minimum_required(VERSION 3.20)
project(shakki4)

set(CMAKE_PREFIX_PATH $CMAKE_PREFIX_PATH $CMAKE_CURRENT_SOURCE_DIR/lib)

set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_BUILD_TYPE Debug)

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og  -Wall -Wextra -fconcepts -DDEBUG=1")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -Wall -Wextra -fconcepts -O3 -flto -DDEBUG=0")
#set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -Wall -Wextra -fconcepts -O3 -flto -DDEBUG=0")




add_executable(shakki4 main.cpp Board.cpp Board.h PieceSet.cpp PieceSet.h BitboardOperations.h BitboardOperations.cpp mytypes.h Bitboard.cpp Bitboard.h Move.cpp Move.h Square.cpp Square.h mytypes.cpp BoardUtils.cpp BoardUtils.h Attacks.cpp Attacks.h StringUtils.cpp StringUtils.h MyMath.cpp MyMath.h SlidingPieceAttacks_impl.h BoardAnalysis.cpp BoardAnalysis.h TerminalColors.h InBetween.cpp InBetween.h CastlingStatus.cpp CastlingStatus.h ai/BoardEvaluator.cpp ai/BoardEvaluator.h ai/Search.cpp ai/Search.h ZobristHash.cpp ZobristHash.h ai/TranspositionTable.cpp ai/TranspositionTable.h Runner.cpp Runner.h Player.cpp Player.h UI.h UI.cpp)
add_executable(tests tests/all.cpp tests/Bitboard.test.cpp tests/Square.test.cpp tests/BoardAnalysis.test.cpp tests/Board.test.cpp tests/TestHelpers.h tests/TestHelpers.cpp tests/Move.test.cpp lib/include/thc/thc.h lib/include/thc/thc.cpp tests/BoardEvaluator.test.cpp tests/Inbetween.test.cpp tests/AI.test.cpp Board.cpp Board.h PieceSet.cpp PieceSet.h BitboardOperations.h BitboardOperations.cpp mytypes.h Bitboard.cpp Bitboard.h Move.cpp Move.h Square.cpp Square.h mytypes.cpp BoardUtils.cpp BoardUtils.h Attacks.cpp Attacks.h StringUtils.cpp StringUtils.h MyMath.cpp MyMath.h SlidingPieceAttacks_impl.h BoardAnalysis.cpp BoardAnalysis.h TerminalColors.h InBetween.cpp InBetween.h CastlingStatus.cpp CastlingStatus.h ai/BoardEvaluator.cpp ai/BoardEvaluator.h ai/Search.cpp ai/Search.h ZobristHash.cpp ZobristHash.h ai/TranspositionTable.cpp ai/TranspositionTable.h Runner.cpp Runner.h Player.cpp Player.h UI.h UI.cpp)

find_package(Catch2 REQUIRED)
target_link_libraries(tests Catch2::Catch2)

include(Catch)
catch_discover_tests(tests)
enable_testing()
