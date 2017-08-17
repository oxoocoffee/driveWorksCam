# DriveWorkls GMSL Camera example
# By Robert J. Gebis (rjgebis@gmail.com)

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( STATUS "In-source builds not allowed. Please make build directory")
    message( STATUS "and run CMake from there")
    message( STATUS "You may need to remove CMakeCache.txt and CmakeFiles/" )
    message( FATAL_ERROR )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

#-----------------------------------------------------------------------
set(PROJ_NAME     "driveWorksCamTest")
set(COMPANY_NAME  "XYZ LLC")

#-----------------------------------------------------------------------
# The version number is used.
set(APP_VERSION_MAJOR      0)
set(APP_VERSION_MINOR      1)
set(APP_VERSION_PATCH      1)
# This one will be set with git revision
set(APP_VERSION_REVISION   0)

# We set temporary version here since project() must be called
# first to initailize cmake subsystem but we will update version later when we test for git repo version
# In this project we only enabling C++ project (CXX). For C based project replace CXX with C or you can mix CXX C
project(${PROJ_NAME} VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.0" LANGUAGES CXX)

add_definitions(-DQT_NO_KEYWORDS)

# Set this variables since they are not used so cmake will not give you warning like
# CMake Warning:
#   Manually-specified variables were not used by the project:
#
#    CMAKE_C_COMPILER
#    QT_QMAKE_EXECUTABLE

set(ignoreMe "${CMAKE_C_COMPILER} ${QT_QMAKE_EXECUTABLE}")

# Check if the given <include> file may be included in a CXX source
include(CheckIncludeFileCXX)

#-----------------------------------------------------------------------
macro(list_files list)
    foreach(infileName ${list})
        message(STATUS " ${infileName}")
    endforeach(infileName)
endmacro()

#-----------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS Off)       # Disable -std=gnu++11 and enable -std=c++11
set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -Wunused-parameter -Wformat-security -Wunreachable-code -fdata-sections -ffunction-sections -Wcast-align")
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wunused-parameter -Wformat-security -Wunreachable-code -fdata-sections -ffunction-sections -Wcast-align")

set(CMAKE_C_FLAGS_DEBUG   "-O0" CACHE INTERNAL "C debug compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "-O0" CACHE INTERNAL "C++ debug compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "-g"  CACHE INTERNAL "ASM debug compiler flags")

set(CMAKE_C_FLAGS_RELEASE   "-Os -flto" CACHE INTERNAL "C release compiler flags")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto" CACHE INTERNAL "C++ release compiler flags")
set(CMAKE_ASM_FLAGS_RELEASE ""          CACHE INTERNAL "ASM release compiler flags")

# If cmake -DCMAKE_BUILD_TYPE=[DEBUG|RELEASE] ..
if( NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "==============================================================")

find_package(Threads REQUIRED)

if(DEFINE_PRINT_FIND_LIB)
    set(LIBFIND_DEBUG YES)
endif(DEFINE_PRINT_FIND_LIB)

set( HAS_DRIVE_WORK 0)

include(ArchConfiguration)

if(VIBRANTE OR LINUX)
    set(LINUX_SYSTEM 1)
else(VIBRANTE OR LINUX)
    set(LINUX_SYSTEM 0)
endif(VIBRANTE OR LINUX)

find_package(Driveworks)

if(Driveworks_FOUND)
    if(CMAKE_CROSSCOMPILING)
        find_package(libusb-1.0 REQUIRED)
        find_package(udev REQUIRED)
        find_package(Xcursor REQUIRED)
        include_directories(${LIBUSB_1_INCLUDE_DIR})
    else(CMAKE_CROSSCOMPILING)
        find_package(GLEW)

        if(NOT GLEW_FOUND)
            message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            message(STATUS "sudo apt-get install libglew-dev")
            message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            message(FATAL_ERROR " You need to install libglew-dev librarty")
        endif(NOT GLEW_FOUND)
    endif(CMAKE_CROSSCOMPILING)

    set(HAS_DRIVE_WORK 1)
    set(DRIVE_WORK_ENABLED YES)
    include_directories(${Driveworks_INCLUDE_DIR})

    #-------------------------------------------------------------------------------
    # Cuda dependencies
    #-------------------------------------------------------------------------------
    if(CMAKE_CROSSCOMPILING)
        set(NVIDIA_CUDA_HACK ${CMAKE_FIND_ROOT_PATH})
        set(CMAKE_FIND_ROOT_PATH "${CMAKE_FIND_ROOT_PATH};/usr/local")
    endif(CMAKE_CROSSCOMPILING)

    find_package(CUDA REQUIRED)

    if(NOT CUDA_FOUND)
        message(STATUS "Please read FindCUDA.cmake about CUDA_BIN_PATH")
        message(FATAL_ERROR " Failed to find CUDA")
    endif(NOT CUDA_FOUND)

    if(CMAKE_CROSSCOMPILING)
        set(CMAKE_FIND_ROOT_PATH ${NVIDIA_CUDA_HACK})
    endif(CMAKE_CROSSCOMPILING)

    include_directories(${CUDA_INCLUDE_DIRS})

    list(APPEND Driveworks_LIBRARIES ${CUDA_LIBRARIES} ${CUDA_cublas_LIBRARY})

    if(LINUX)
        list(APPEND Driveworks_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} dl rt)
    elseif(VIBRANTE)
        list(APPEND Driveworks_LIBRARIES ${vibrante_LIBRARIES} nvmedia ${CMAKE_THREAD_LIBS_INIT})
    endif()

else(Driveworks_FOUND)
    set(HAS_DRIVE_WORK 0)
    set(DRIVE_WORK_ENABLED NO)
endif(Driveworks_FOUND)

string( TOUPPER ${CMAKE_BUILD_TYPE} DEFINE_DEBUG_BUILD)

# AB_BUILD_TYPE is used to flag source code DEBUG definition
if(DEFINE_DEBUG_BUILD STREQUAL DEBUG)
    set(DEFINE_DEBUG_BUILD "1")
    add_definitions(-DDEBUG)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    endif()
else()
    set(DEFINE_DEBUG_BUILD "0")
    add_definitions(-DRELEASE)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/buildVersion.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/include/buildVersion.h @ONLY)

#-----------------------------------------------------------------------
# Add include folder to compiler search path so foo.h can be found
include_directories(include ../include)

# This is needed to show common include files in Qt Cretor Project Tree
FILE(GLOB_RECURSE HeadFiles "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_custom_target(headers SOURCES ${HeadFiles})

# This is needed to show common include files in Qt Cretor Project Tree
FILE(GLOB_RECURSE ReadmeFiles "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")

add_custom_target(README SOURCES ${ReadmeFiles})

# There are two ways including your sources in project
# Manual or Automatic. In Manual Mode you add files that you want to include
# Automatic will add all cpp files found in src

# Automatic
file(GLOB app_SRCS src/*.cpp include/*.h)

list_files( "${app_SRCS}" )

add_executable(${PROJECT_NAME} ${app_SRCS})

if(CMAKE_CROSSCOMPILING AND VIBRANTE)
    set(CAMERA_VIEWER_LIBS ${AB_CAMERA_VIEWER_LIBS} ${XCURSOR_LIBRARY} ${UDEV_LIBRARY} ${LIBUSB_1_LIBRARY})
endif(CMAKE_CROSSCOMPILING AND VIBRANTE)

target_link_libraries(${PROJECT_NAME} ${Driveworks_LIBRARIES} ${CAMERA_VIEWER_LIBS} pthread)
