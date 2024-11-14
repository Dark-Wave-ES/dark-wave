cmake_minimum_required(VERSION 3.21)
project(file-encryption-system LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
      Poco
      GIT_REPOSITORY https://github.com/pocoproject/poco.git
      GIT_TAG poco-1.13.4
      FIND_PACKAGE_ARGS NAMES Poco
)

FetchContent_Declare(
      Slint
      GIT_REPOSITORY https://github.com/slint-ui/slint.git
      GIT_TAG release/1.8
      SOURCE_SUBDIR api/cpp
      FIND_PACKAGE_ARGS NAMES Slint
)

FetchContent_MakeAvailable(Poco Slint)

set(PROJECT_SOURCES
        src/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Crypto Slint::Slint)
slint_target_sources(${PROJECT_NAME} ui/app-window.slint)
