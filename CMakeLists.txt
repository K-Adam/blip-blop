cmake_minimum_required(VERSION 3.18)

project(blipblop CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SRC CONFIGURE_DEPENDS src/*.cpp src/menus/*.cpp)

add_executable(blipblop ${SRC})
target_include_directories(blipblop PRIVATE include)

if(MSVC)
    target_compile_options(blipblop PRIVATE /W3 /utf-8)
else()
    target_compile_options(blipblop PRIVATE -Wall)
endif()

if(EMSCRIPTEN)
    set(SDL_PORTS -sUSE_SDL=2 -sUSE_SDL_MIXER=2 -sUSE_SDL_IMAGE=2 "-sSDL2_IMAGE_FORMATS=[\"bmp\",\"png\"]")
    target_compile_options(blipblop PRIVATE ${SDL_PORTS} $<$<CONFIG:Debug>:-fexceptions -sNO_DISABLE_EXCEPTION_CATCHING>)
    target_link_options(blipblop PRIVATE ${SDL_PORTS} -sASYNCIFY -sALLOW_MEMORY_GROWTH -lidbfs.js
        "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/assets@assets" "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/config@config"
        $<$<CONFIG:Debug>:-gsource-map -fexceptions -sNO_DISABLE_EXCEPTION_CATCHING -sEXCEPTION_DEBUG=1>)
    set_target_properties(blipblop PROPERTIES OUTPUT_NAME "index" SUFFIX ".html")
elseif(WIN32)
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(blipblop PRIVATE SDL2::SDL2)
    # Plain library lookup because some vcpkg SDL2_image configs demand codecs that were not built
    foreach(lib SDL2_image SDL2_mixer)
        find_library(${lib}_RELEASE ${lib} REQUIRED)
        find_library(${lib}_DEBUG NAMES ${lib}d ${lib} REQUIRED)
        target_link_libraries(blipblop PRIVATE optimized ${${lib}_RELEASE} debug ${${lib}_DEBUG})
    endforeach()
else()
    target_link_libraries(blipblop PRIVATE SDL2 SDL2_image SDL2_mixer)
endif()

if(NOT EMSCRIPTEN)
    option(BUILD_TESTING "Build the end to end tests" ON)
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
