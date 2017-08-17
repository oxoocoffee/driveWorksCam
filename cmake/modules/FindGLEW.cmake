#  GLEW_FOUND - system has yaml-cpp
#  GLEW_INCLUDE_DIRS - the yaml-cpp include directory
#  GLEW_LIBRARIES - Link these to use yaml-cpp
#  GLEW_DEFINITIONS - Compiler switches required for using yaml-cpp

message(STATUS "Looking for glew")

if (GLEW_LIBRARIES AND GLEW_INCLUDE_DIRS)
  # in cache already
  set(GLEW_FOUND TRUE)
else (GLEW_LIBRARIES AND GLEW_INCLUDE_DIRS)
    find_path(  GLEW_INCLUDE_DIR
                NAMES glew.h
                PATHS /usr/local/include /usr/include
                PATH_SUFFIXES GL)

    find_library( GLEW_LIBRARY
                NAMES GLEW
                PATHS /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu)

    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
    set(GLEW_LIBRARIES ${GLEW_LIBRARY} )

    if (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
        set(GLEW_FOUND TRUE)
    endif (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)

    if (NOT GLEW_FOUND)
        if (GLEW_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find glew")
        endif (GLEW_FIND_REQUIRED)
    endif (NOT GLEW_FOUND)

    # show the GLEW_INCLUDE_DIRS and GLEW_LIBRARIES variables only in the advanced view
    mark_as_advanced(GLEW_INCLUDE_DIRS GLEW_LIBRARIES)

endif (GLEW_LIBRARIES AND GLEW_INCLUDE_DIRS)

if(GLEW_FOUND)
    message(STATUS "Looking for glew - found")
    if (LIBFIND_DEBUG)
        message(STATUS "  GLEW_INCLUDE_DIR=${GLEW_INCLUDE_DIR}")
        message(STATUS "  GLEW_LIBRARY=${GLEW_LIBRARY}")
        message(STATUS "  GLEW_DEFINITIONS=${GLEW_DEFINITIONS}")
    endif(LIBFIND_DEBUG)
endif(GLEW_FOUND)
