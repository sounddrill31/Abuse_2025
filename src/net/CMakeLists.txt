add_library(net STATIC
    gserver.cpp gserver.h
    gclient.cpp gclient.h
    fileman.cpp fileman.h
    sock.cpp sock.h
    tcpip.cpp tcpip.h
    ghandler.h netface.h
)

include_directories(
    ${abuse_SOURCE_DIR}/src
    ${abuse_SOURCE_DIR}/src/imlib
)

if(EMSCRIPTEN)
    # Emscripten ports are linked via flags, not CMake targets
    target_link_options(net PRIVATE
        "SHELL:-sUSE_SDL=2"
        "SHELL:-sUSE_SDL_MIXER=2"
        "SHELL:-sSDL2_MIXER_FORMATS=['ogg','mod','wav']"
    )
else()
    # Native or vcpkg builds link to actual imported targets
    target_link_libraries(net
        PRIVATE
        SDL2::SDL2
        SDL2_mixer::SDL2_mixer
    )
endif()

add_definitions(-DNO_CHECK)

if(WIN32)
    target_link_libraries(net ws2_32)
endif()
