#  UDEV_FOUND - system has libudev
#  UDEV_INCLUDE_DIRS - the libudev include directory
#  UDEV_LIBRARIES - Link these to use libudev
#  UDEV_DEFINITIONS - Compiler switches required for using libudev

message(STATUS "Looking for libudev")

if (UDEV_LIBRARIES AND UDEV_INCLUDE_DIRS)
  # in cache already
  set(UDEV_FOUND TRUE)
else (UDEV_LIBRARIES AND UDEV_INCLUDE_DIRS)

    find_library(UDEV_LIBRARY
                 NAMES udev
                 PATHS /usr/local/lib /usr/lib )

    set(UDEV_LIBRARIES ${UDEV_LIBRARY})

    if( NOT CMAKE_CROSSCOMPILING AND NOT VIBRANTE)
        find_path(  UDEV_INCLUDE_DIR
                    NAMES libudev.h
                    PATHS /usr/local/include /usr/include
                    PATH_SUFFIXES libdev)

        set(UDEV_INCLUDE_DIRS ${UDEV_INCLUDE_DIR} )

        if (UDEV_INCLUDE_DIRS AND UDEV_LIBRARIES)
            set(UDEV_FOUND TRUE)
        endif (UDEV_INCLUDE_DIRS AND UDEV_LIBRARIES)

    else(NOT CMAKE_CROSSCOMPILING AND NOT VIBRANTE)
        message(STATUS "Drivework cross compilation does not provides libudev.h")
        set(UDEV_INCLUDE_DIRS)

        if ( UDEV_LIBRARIES)
            set(UDEV_FOUND TRUE)
        endif (UDEV_LIBRARIES)

    endif(NOT CMAKE_CROSSCOMPILING AND NOT VIBRANTE)

    if (NOT UDEV_FOUND)
        if (UDEV_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find libudev")
        endif (UDEV_FIND_REQUIRED)
    endif (NOT UDEV_FOUND)

    # show the UDEV_INCLUDE_DIRS and UDEV_LIBRARIES variables only in the advanced view
    mark_as_advanced(UDEV_INCLUDE_DIRS UDEV_LIBRARIES)

endif (UDEV_LIBRARIES AND UDEV_INCLUDE_DIRS)

if(UDEV_FOUND)
    message(STATUS "Looking for libudev - found")
    if (LIBFIND_DEBUG)
        message(STATUS "  UDEV_INCLUDE_DIR=${UDEV_INCLUDE_DIR}")
        message(STATUS "  UDEV_LIBRARY=${UDEV_LIBRARY}")
        message(STATUS "  UDEV_DEFINITIONS=${UDEV_DEFINITIONS}")
    endif(LIBFIND_DEBUG)
endif(UDEV_FOUND)
