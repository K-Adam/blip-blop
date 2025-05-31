include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(e2e_tests e2e_test.cpp)
target_link_libraries(e2e_tests PRIVATE GTest::gtest_main)
target_compile_definitions(e2e_tests PRIVATE GAME_EXE="$<TARGET_FILE:blipblop>")
add_dependencies(e2e_tests blipblop)

include(GoogleTest)
# BB_FIXEDSTEP ties game time to frames so the scripted input lands on the same screens every run
gtest_discover_tests(e2e_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DISCOVERY_MODE PRE_TEST
    PROPERTIES
        ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy;BB_FIXEDSTEP=1"
        TIMEOUT 120)
