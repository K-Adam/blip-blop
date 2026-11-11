include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(game_tests game_test.cpp)
target_link_libraries(game_tests PRIVATE blipblop_core GTest::gtest_main)

include(GoogleTest)
# Assets load by relative path
gtest_discover_tests(game_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DISCOVERY_MODE PRE_TEST
    PROPERTIES
        TIMEOUT 120)
