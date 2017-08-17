#  XCURSOR_FOUND - system has libusb
#  XCURSOR_INCLUDE_DIRS - the libusb include directory
#  XCURSOR_LIBRARIES - Link these to use libusb
#  XCURSOR_DEFINITIONS - Compiler switches required for using libusb

message(STATUS "Looking for libXcursor")

if (XCURSOR_LIBRARIES AND XCURSOR_INCLUDE_DIRS)
  # in cache already
  set(LIBUSB_FOUND TRUE)
else (XCURSOR_LIBRARIES AND XCURSOR_INCLUDE_DIRS)

    find_library(XCURSOR_LIBRARY
                 NAMES Xcursor
                 PATHS /usr/local/lib /usr/lib  )

    set(XCURSOR_LIBRARIES ${XCURSOR_LIBRARY})

    if( NOT CMAKE_CROSSCOMPILING AND NOT VIBRANTE)
        find_path(  XCURSOR_INCLUDE_DIR
                    NAMES Xcursor.h
                    PATHS /usr/local/include /usr/include
                    PATH_SUFFIXES libXcursor)

        set(XCURSOR_INCLUDE_DIRS ${XCURSOR_INCLUDE_DIR} )

        if (XCURSOR_INCLUDE_DIRS AND XCURSOR_LIBRARIES)
            set(XCURSOR_FOUND TRUE)
        endif (XCURSOR_INCLUDE_DIRS AND XCURSOR_LIBRARIES)

    else(NOT CMAKE_CROSSCOMPILING AND NOT VIBRANTE)
        message(STATUS "Drivework cross compilation does not provides Xcursor.h")
        set(XCURSOR_INCLUDE_DIRS)

        if ( XCURSOR_LIBRARIES)
            set(XCURSOR_FOUND TRUE)
        endif (XCURSOR_LIBRARIES)

    endif(NOT CMAKE_CROSSCOMPILING AND NOT VIBRANTE)

    if (NOT XCURSOR_FOUND)
        if (XCURSOR_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find libXcursor")
        endif (XCURSOR_FIND_REQUIRED)
    endif (NOT XCURSOR_FOUND)

    # show the XCURSOR_INCLUDE_DIRS and XCURSOR_LIBRARIES variables only in the advanced view
    mark_as_advanced(XCURSOR_INCLUDE_DIRS XCURSOR_LIBRARIES)

endif (XCURSOR_LIBRARIES AND XCURSOR_INCLUDE_DIRS)

if(XCURSOR_FOUND)
    message(STATUS "Looking for libXcursor - found")
    if (LIBFIND_DEBUG)
        message(STATUS "  XCURSOR_INCLUDE_DIR=${XCURSOR_INCLUDE_DIR}")
        message(STATUS "  XCURSOR_LIBRARY=${XCURSOR_LIBRARY}")
        message(STATUS "  XCURSOR_DEFINITIONS=${XCURSOR_DEFINITIONS}")
    endif(LIBFIND_DEBUG)
endif(XCURSOR_FOUND)
