include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(e2e_tests startup_test.cpp)
target_link_libraries(e2e_tests PRIVATE GTest::gtest_main)
target_compile_definitions(e2e_tests PRIVATE GAME_EXE="$<TARGET_FILE:blipblop>")
add_dependencies(e2e_tests blipblop)

add_test(NAME e2e_tests COMMAND e2e_tests)
set_tests_properties(e2e_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy"
    TIMEOUT 120)
