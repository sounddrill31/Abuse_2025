add_library(lisp STATIC
    lisp.cpp lisp.h
    lisp_opt.cpp lisp_opt.h
    lisp_gc.cpp lisp_gc.h
    trig.cpp
    stack.h symbols.h
)

include_directories(
    ${abuse_SOURCE_DIR}/src
    ${abuse_SOURCE_DIR}/src/imlib
)


if(EMSCRIPTEN)
    # Emscripten ports are linked via flags, not CMake targets
    target_link_options(lisp PRIVATE
        "SHELL:-sUSE_SDL=2"
        "SHELL:-sUSE_SDL_MIXER=2"
        "SHELL:-sSDL2_MIXER_FORMATS=['ogg','mod','wav']"
    )
else()
    # Native or vcpkg builds link to actual imported targets
    target_link_libraries(lisp
        PRIVATE
        SDL2::SDL2
        SDL2_mixer::SDL2_mixer
    )
endif()

#INCLUDES = -I$(srcdir) -I$(srcdir)/../ -I$(srcdir)/../imlib $(SDL2_CFLAGS)

#CPPFLAGS = @CPPFLAGS@ -DNO_CHECK
