#DEFS = @DEFS@

add_library(sdlport STATIC
    video.cpp
    event.cpp
    sound.cpp sound.h
    timing.cpp
    jdir.cpp
    joystick.cpp joy.h
    setup.cpp setup.h
    hmi.cpp hmi.h
    errorui.cpp errorui.h
)
#libsdlport_a_LIBADD =
#
include_directories(
    ${abuse_SOURCE_DIR}/src
    ${abuse_SOURCE_DIR}/src/imlib
)

if (EMSCRIPTEN)
    # Emscripten ports - these are handled automatically via compiler flags
    target_link_options(sdlport PRIVATE
        "SHELL:-sUSE_SDL=2"
        "SHELL:-sUSE_SDL_MIXER=2"
        "SHELL:-sSDL2_MIXER_FORMATS=['ogg','mod','wav']"
    )
else()
    # For native or vcpkg builds, link to proper imported targets
    target_link_libraries(sdlport
        PRIVATE
        SDL2::SDL2
        SDL2_mixer::SDL2_mixer
    )
endif()

#
#CPPFLAGS = @CPPFLAGS@ -DNO_CHECK -DASSETDIR=\"$(assetdir)\"
