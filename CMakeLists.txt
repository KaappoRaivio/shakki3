cmake_minimum_required(VERSION 3.20)
project(shakki3)

set(CMAKE_PREFIX_PATH $CMAKE_PREFIX_PATH $CMAKE_CURRENT_SOURCE_DIR/lib)

set(CMAKE_CXX_STANDARD 23)

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og  -Wall -Wextra -fconcepts -DDEBUG=1")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -Wall -Wextra -fconcepts -O0 -DDEBUG=1")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -Wall -Wextra -fconcepts -O3 -flto -DDEBUG=0")




add_executable(shakki3 main.cpp src/Board.cpp src/Board.h src/PieceSet.cpp src/PieceSet.h src/BitboardOperations.h src/BitboardOperations.cpp src/mytypes.h src/Bitboard.cpp src/Bitboard.h src/Move.cpp src/Move.h src/Square.cpp src/Square.h src/mytypes.cpp src/BoardUtils.cpp src/BoardUtils.h src/Attacks.cpp src/Attacks.h src/StringUtils.cpp src/StringUtils.h src/MyMath.cpp src/MyMath.h src/SlidingPieceAttacks_impl.h src/BoardAnalysis.cpp src/BoardAnalysis.h src/TerminalColors.h src/InBetween.cpp src/InBetween.h src/CastlingStatus.cpp src/CastlingStatus.h src/ai/BoardEvaluator.cpp src/ai/BoardEvaluator.h src/ai/Search.cpp src/ai/Search.h src/ZobristHash.cpp src/ZobristHash.h src/ai/TranspositionTable.cpp src/ai/TranspositionTable.h src/Runner.cpp src/Runner.h src/Player.cpp src/Player.h src/ui/UI.h src/ui/UI.cpp src/ui/HTTPUI.cpp src/ui/HTTPUI.h)
add_executable(tests tests/all.cpp tests/Bitboard.test.cpp tests/Square.test.cpp tests/BoardAnalysis.test.cpp tests/Board.test.cpp tests/TestHelpers.h tests/TestHelpers.cpp tests/Move.test.cpp lib/include/thc/thc.h lib/include/thc/thc.cpp tests/BoardEvaluator.test.cpp tests/Inbetween.test.cpp tests/AI.test.cpp src/Board.cpp src/Board.h src/PieceSet.cpp src/PieceSet.h src/BitboardOperations.h src/BitboardOperations.cpp src/mytypes.h src/Bitboard.cpp src/Bitboard.h src/Move.cpp src/Move.h src/Square.cpp src/Square.h src/mytypes.cpp src/BoardUtils.cpp src/BoardUtils.h src/Attacks.cpp src/Attacks.h src/StringUtils.cpp src/StringUtils.h src/MyMath.cpp src/MyMath.h src/SlidingPieceAttacks_impl.h src/BoardAnalysis.cpp src/BoardAnalysis.h src/TerminalColors.h src/InBetween.cpp src/InBetween.h src/CastlingStatus.cpp src/CastlingStatus.h src/ai/BoardEvaluator.cpp src/ai/BoardEvaluator.h src/ai/Search.cpp src/ai/Search.h src/ZobristHash.cpp src/ZobristHash.h src/ai/TranspositionTable.cpp src/ai/TranspositionTable.h src/Runner.cpp src/Runner.h src/Player.cpp src/Player.h src/ui/UI.h src/ui/UI.cpp)


include_directories(lib/include)

find_package(Catch2 3 REQUIRED)

target_link_libraries(tests Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
enable_testing()