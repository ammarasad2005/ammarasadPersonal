cmake_minimum_required(VERSION 3.16)
project(HangmanGame VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-all>=2.5)

# Add executable
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/WordManager.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
    src/UIManager.cpp
    src/SoundManager.cpp
)

add_executable(hangman ${SOURCES})

# Link SFML
target_link_libraries(hangman ${SFML_LIBRARIES})
target_include_directories(hangman PRIVATE ${SFML_INCLUDE_DIRS})
target_compile_options(hangman PRIVATE ${SFML_CFLAGS_OTHER})

# Copy assets to build directory
add_custom_command(TARGET hangman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:hangman>/assets)

# Set build flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(hangman PRIVATE -g -Wall -Wextra)
else()
    target_compile_options(hangman PRIVATE -O3)
endif()