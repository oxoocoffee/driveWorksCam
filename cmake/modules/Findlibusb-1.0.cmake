#  LIBUSB_1_FOUND - system has libusb
#  LIBUSB_1_INCLUDE_DIRS - the libusb include directory
#  LIBUSB_1_LIBRARIES - Link these to use libusb
#  LIBUSB_1_DEFINITIONS - Compiler switches required for using libusb

message(STATUS "Looking for libusb-1.0")

if (LIBUSB_1_LIBRARIES AND LIBUSB_1_INCLUDE_DIRS)
  # in cache already
  set(LIBUSB_FOUND TRUE)
else (LIBUSB_1_LIBRARIES AND LIBUSB_1_INCLUDE_DIRS)
    find_path(  LIBUSB_1_INCLUDE_DIR
                NAMES libusb.h
                PATHS /usr/local/include /usr/include
                PATH_SUFFIXES libusb-1.0)

    find_library( LIBUSB_1_LIBRARY
                NAMES usb-1.0 usb
                PATHS /usr/local/lib /usr/lib )

    set(LIBUSB_1_INCLUDE_DIRS ${LIBUSB_1_INCLUDE_DIR})
    set(LIBUSB_1_LIBRARIES ${LIBUSB_1_LIBRARY} )

    if (LIBUSB_1_INCLUDE_DIRS AND LIBUSB_1_LIBRARIES)
        set(LIBUSB_1_FOUND TRUE)
    endif (LIBUSB_1_INCLUDE_DIRS AND LIBUSB_1_LIBRARIES)

    if (NOT LIBUSB_1_FOUND)
        if (libusb_1_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find libusb")
        endif (libusb_1_FIND_REQUIRED)
    endif (NOT LIBUSB_1_FOUND)

    # show the LIBUSB_1_INCLUDE_DIRS and LIBUSB_1_LIBRARIES variables only in the advanced view
    mark_as_advanced(LIBUSB_1_INCLUDE_DIRS LIBUSB_1_LIBRARIES)

endif (LIBUSB_1_LIBRARIES AND LIBUSB_1_INCLUDE_DIRS)

if(LIBUSB_1_FOUND)
    message(STATUS "Looking for libusb-1.0 - found")
    if (LIBFIND_DEBUG)
        message(STATUS "  LIBUSB_1_INCLUDE_DIR=${LIBUSB_1_INCLUDE_DIR}")
        message(STATUS "  LIBUSB_1_LIBRARY=${LIBUSB_1_LIBRARY}")
        message(STATUS "  LIBUSB_1_DEFINITIONS=${LIBUSB_1_DEFINITIONS}")
    endif(LIBFIND_DEBUG)
endif(LIBUSB_1_FOUND)
