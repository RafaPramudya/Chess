set(CHESS_SRC "Chess.cpp")

file(GLOB_RECURSE CHESS_GAME_SRC "Game/*.cpp")
add_executable (ChessGame ${CHESS_SRC} ${CHESS_GAME_SRC})

target_include_directories(ChessGame PRIVATE "/" "Game")
target_link_libraries(ChessGame PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

file(GLOB_RECURSE CHESS_BOT_SRC "Mind++/*.cpp")
add_executable (ChessBot ${CHESS_SRC} ${CHESS_BOT_SRC})
target_include_directories(ChessBot PRIVATE "/" "Mind++")

if (CMAKE_VERSION VERSION_GREATER 3.16)
  set_property(TARGET ChessGame PROPERTY CXX_STANDARD 23)
  set_property(TARGET ChessBot PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
