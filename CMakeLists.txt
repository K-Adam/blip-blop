cmake_minimum_required(VERSION 3.1)

project(blipblop)

find_package(SDL2 REQUIRED)
find_library(SDL2_LIBRARIES SDL2)
find_library(SDL2IMAGE_LIBRARIES SDL2_image)
find_library(SDL2MIXER_LIBRARIES SDL2_mixer)
find_path(SDL2_INCLUDE_DIRS SDL2/SDL.h)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

aux_source_directory(src SRC)
aux_source_directory(src/menus MENUS_SRC)

include_directories(include)

if(WIN32)
    add_executable(blipblop ${SRC} ${MENUS_SRC})
    target_link_libraries(blipblop PUBLIC ${SDL2_LIBRARIES})
    target_link_libraries(blipblop PUBLIC ${SDL2IMAGE_LIBRARIES})
    target_link_libraries(blipblop PUBLIC ${SDL2MIXER_LIBRARIES})
    target_include_directories(blipblop SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS})

    set(CMAKE_INSTALL_UCRT_LIBRARIES true)
    include(InstallRequiredSystemLibraries)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/\${CMAKE_INSTALL_CONFIG_NAME}/"
        DESTINATION .
        FILES_MATCHING PATTERN "*.dll")
elseif(EMSCRIPTEN)
    add_executable(blipblop WIN32 ${SRC} ${MENUS_SRC})

    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2 -sUSE_SDL_MIXER=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='[\"bmp\",\"png\"]'")
else()
    add_executable(blipblop WIN32 ${SRC} ${MENUS_SRC})
    target_link_libraries(blipblop SDL2)

    target_link_libraries(blipblop SDL2_image)
    target_link_libraries(blipblop SDL2_mixer)
endif()

if(EMSCRIPTEN)
    set(PRELOAD_FILE_OPTION "--preload-file assets --preload-file config")
    set(ADDITIONAL_LINK_FLAGS "-sASYNCIFY -sALLOW_MEMORY_GROWTH")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g -gsource-map -fexceptions -sNO_DISABLE_EXCEPTION_CATCHING")
        set(ADDITIONAL_LINK_FLAGS "${ADDITIONAL_LINK_FLAGS} -s EXCEPTION_DEBUG=1")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()

    set_target_properties(blipblop PROPERTIES OUTPUT_NAME "index")
    set_target_properties(blipblop PROPERTIES LINK_FLAGS "${PRELOAD_FILE_OPTION} ${ADDITIONAL_LINK_FLAGS}")
else()
    install(DIRECTORY ./assets DESTINATION .)
endif()

install(TARGETS blipblop RUNTIME DESTINATION .)

set(CPACK_PACKAGE_NAME "BlipnBlop")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Blip & Blop revamped")
set(CPACK_PACKAGE_VENDOR "Loaded Studios")
set(CPACK_DEBIAN_PACKAGE_NAME "blipnblop")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libSDL2, libSD2-mixer")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Blip & Blop revamped")
set(CPACK_PACKAGE_EXECUTABLES "blipblop" "BlipnBlop")

if(WIN32)
    set(CPACK_NSIS_DISPLAY_NAME "Blip & Blop")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
endif()

include(CPack)
