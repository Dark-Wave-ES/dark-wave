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

set(PROJECT_HEADERS
        src/config.h
        src/crypto/symmetric-encryption.h
        src/services/encryption/symmetric-encryption-service.h
        src/utils/pickers/pickers.h
        src/utils/salt-generator/salt-generator.h
        src/utils/base64/base64.h
        src/crypto/kdf/kdf.h
        src/crypto/aes/aes-gcm/aes-gcm.h
)

set(PROJECT_SOURCES
        src/services/encryption/symmetric-encryption-service.cpp
        src/utils/pickers/pickers.cpp
        src/utils/salt-generator/salt-generator.cpp
        src/utils/base64/base64.cpp
        src/crypto/kdf/kdf.cpp
        src/crypto/aes/aes-gcm/aes-gcm.cpp
        src/main.cpp
)

set(UNIT_TESTS
        tests/unit/salt-generator/salt-generator-tests.cpp
        tests/unit/base64/base64-tests.cpp
        tests/unit/aes/aes-tests.cpp
)

set(UNIT_TEST_HEADERS
        src/utils/salt-generator/salt-generator.h
        src/utils/base64/base64.h
        src/crypto/symmetric-encryption.h
        src/services/encryption/symmetric-encryption-service.h

)

set(UNIT_TEST_SOURCES
        src/utils/salt-generator/salt-generator.cpp
        src/utils/base64/base64.cpp
        src/services/encryption/symmetric-encryption-service.cpp
        src/crypto/aes/aes-gcm/aes-gcm.cpp
)

set(INTEGRATION_TEST_HEADERS ${PROJECT_HEADERS} tests/integration/integration-test.h)

set(INCLUDE_DIRECTORIES
        src
        src/crypto
        src/crypto/aes/aes-gcm
        src/services/encryption
        src/utils
        src/utils/pickers
        src/utils/base64
        src/utils/salt-generator
)

enable_testing()

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
add_executable(unit_tests ${UNIT_TESTS} ${UNIT_TEST_HEADERS} ${UNIT_TEST_SOURCES})

add_library(ui)
target_link_libraries(ui PRIVATE Slint::Slint)
slint_target_sources(ui ui/app.slint)

target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Crypto Slint::Slint ui)
target_link_libraries(unit_tests PRIVATE gtest_main gtest gmock Poco::Crypto)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(unit_tests PRIVATE ${INCLUDE_DIRECTORIES})

add_test(NAME UnitTests COMMAND unit_tests)
