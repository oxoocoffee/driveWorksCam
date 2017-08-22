# cat /proc/cpuinfo
# model name      : ARMv8 Processor rev 3 (v8l)
# BogoMIPS        : 62.50
# Features        : fp asimd evtstrm aes pmull sha1 sha2 crc32
# CPU implementer : 0x41
# CPU architecture: 8
# CPU variant     : 0x1
# CPU part        : 0xd07  (Cortex A57)
# CPU revision    : 3

include(Toolchain-V4L)

set(CMAKE_SYSTEM_VERSION vibrante)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)

set(CROSS_PATH "/usr/local/driveworks/samples/3rdparty/linux-aarch64/vibrante;/opt/cross-arm/dw-px2/sysroot")

set(CMAKE_FIND_ROOT_PATH ${CROSS_PATH} ${CMAKE_FIND_ROOT_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


