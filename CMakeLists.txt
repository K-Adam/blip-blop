cmake_minimum_required(VERSION 3.18)

project(blipblop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM SRC ${CMAKE_SOURCE_DIR}/src/core/main.cpp)

# OBJECT so the tests link every translation unit the game does
add_library(blipblop_core OBJECT ${SRC})
target_include_directories(blipblop_core PUBLIC src)
# Vendored third party headers, their warnings are not ours to fix
target_include_directories(blipblop_core SYSTEM PUBLIC include)

if(MSVC)
    target_compile_options(blipblop_core PUBLIC /W4 /utf-8)
else()
    target_compile_options(blipblop_core PUBLIC -Wall -Wextra)
endif()

add_executable(blipblop src/core/main.cpp)
target_link_libraries(blipblop PRIVATE blipblop_core)

if(EMSCRIPTEN)
    set(SDL_PORTS -sUSE_SDL=2 -sUSE_SDL_MIXER=2 -sUSE_SDL_IMAGE=2 "-sSDL2_IMAGE_FORMATS=[\"bmp\",\"png\"]")
    target_compile_options(blipblop_core PUBLIC ${SDL_PORTS} $<$<CONFIG:Debug>:-fexceptions -sNO_DISABLE_EXCEPTION_CATCHING>)
    target_link_options(blipblop PRIVATE ${SDL_PORTS} -sALLOW_MEMORY_GROWTH -lidbfs.js
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/web/shell.html" "SHELL:--pre-js ${CMAKE_SOURCE_DIR}/web/pre.js"
        "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/assets@assets" "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/config@config_defaults"
        $<$<CONFIG:Debug>:-gsource-map -fexceptions -sNO_DISABLE_EXCEPTION_CATCHING -sEXCEPTION_DEBUG=1>)
    set_target_properties(blipblop PROPERTIES OUTPUT_NAME "index" SUFFIX ".html")
    set_property(TARGET blipblop APPEND PROPERTY LINK_DEPENDS ${CMAKE_SOURCE_DIR}/web/shell.html ${CMAKE_SOURCE_DIR}/web/pre.js)
elseif(WIN32)
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(blipblop_core PUBLIC SDL2::SDL2)
    # Plain library lookup because some vcpkg SDL2_image configs demand codecs that were not built
    foreach(lib SDL2_image SDL2_mixer)
        find_library(${lib}_RELEASE ${lib} REQUIRED)
        find_library(${lib}_DEBUG NAMES ${lib}d ${lib} REQUIRED)
        target_link_libraries(blipblop_core PUBLIC optimized ${${lib}_RELEASE} debug ${${lib}_DEBUG})
    endforeach()
else()
    target_link_libraries(blipblop_core PUBLIC SDL2 SDL2_image SDL2_mixer)
endif()

if(NOT EMSCRIPTEN)
    option(BUILD_TESTING "Build the integration tests" ON)
    if(BUILD_TESTING)
        enable_testing()
        add_subdirectory(tests)
    endif()

    install(TARGETS blipblop RUNTIME DESTINATION .)
    install(DIRECTORY assets config DESTINATION .)
    if(WIN32)
        set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
        include(InstallRequiredSystemLibraries)
        install(DIRECTORY $<TARGET_FILE_DIR:blipblop>/ DESTINATION . FILES_MATCHING PATTERN "*.dll")
    endif()
endif()

set(CPACK_PACKAGE_NAME "BlipnBlop")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Blip & Blop revamped")
set(CPACK_PACKAGE_VENDOR "Loaded Studios")
set(CPACK_DEBIAN_PACKAGE_NAME "blipnblop")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0, libsdl2-image-2.0-0, libsdl2-mixer-2.0-0")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Blip & Blop revamped")
set(CPACK_PACKAGE_EXECUTABLES "blipblop" "BlipnBlop")
set(CPACK_NSIS_DISPLAY_NAME "Blip & Blop")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")

include(CPack)
