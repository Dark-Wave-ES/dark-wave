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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

configure_file(CMakeLists.txt.googletest googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build . RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(PROJECT_SOURCES
        src/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Crypto Slint::Slint)
slint_target_sources(${PROJECT_NAME} ui/app-window.slint)
