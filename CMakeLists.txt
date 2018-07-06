#
# project: dspr-frontend
#
cmake_minimum_required(VERSION 2.8)

set(FIPS_EXCEPTIONS ON CACHE BOOL "Enable C++ exceptions" FORCE)

#the following variable is set after emscripten.toolchain.cmake does it's thing...
#if browser is throwing exception that no getSocketFromFd() function exists
#1. Go to /fips/cmake-toolchains/emscripten.toolchain.cmake
#2. Paste the following line in a newline at line 109
set(FIPS_EMSCRIPTEN_USE_FS ON)
#3. Rebuild

get_filename_component(emsc_libs_path ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(emsc_libs_path "${emsc_libs_path}/fips-sdks/linux/emsdk-portable/emscripten/incoming/src")
set(emsc_libs "${emsc_libs} ${emsc_libs_path}/library_syscall.js")
set(emsc_libs "${emsc_libs} ${emsc_libs_path}/library_sockfs.js")
set(emsc_libs "${emsc_libs} ${emsc_libs_path}/library_fs.js")
set(emsc_libs "${emsc_libs} ${emsc_libs_path}/library_tty.js")
set(emsc_libs "${emsc_libs} ${emsc_libs_path}/library_memfs.js")

string(REPLACE " " ";" emsc_libs_list ${emsc_libs})
foreach(emsc_lib ${emsc_libs_list})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --js-library ${emsc_lib}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --js-library ${emsc_lib}")
endforeach()

# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

# this hack is required for turbobadger since we need to make
# sure that our overriden config.h is included
include_directories(../oryol-tbui/src/TBUI/tb)

fips_setup(PROJECT dspr-frontend)
include_directories(src)

fips_add_subdirectory(src)
fips_finish()


