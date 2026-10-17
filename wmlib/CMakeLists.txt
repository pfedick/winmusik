cmake_minimum_required(VERSION 3.20)
project(wmlib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)              # -> gnu++20 wie bisher
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

# ==============================================================================
# pplib7 (Alt, Autoconf) - Uebergangsphase
# ==============================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PPLIB7_DIR ../pplib/debug)
else()
    set(PPLIB7_DIR ../pplib/release)
endif()
add_library(pplib7 STATIC IMPORTED)
set_target_properties(pplib7 PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../pplib/${PPLIB7_DIR}/libpplib.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../pplib/include
)

# ==============================================================================
# pplib8 (CMake, vorgelagert in build/pplib8-<cfg> konfiguriert)
# ==============================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PPLIB8_BUILD debug)
else()
    set(PPLIB8_BUILD release)
endif()

set(PPLIB8_ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/../pplib8)
set(PPLIB8_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build/pplib8-${PPLIB8_BUILD})

foreach(mod core audio internet)
    add_library(pplib::${mod} STATIC IMPORTED)
    set_target_properties(pplib::${mod} PROPERTIES
        IMPORTED_LOCATION ${PPLIB8_BUILD_DIR}/libpplib_${mod}.a
        INTERFACE_INCLUDE_DIRECTORIES ${PPLIB8_ROOT}/include
    )
endforeach()
# Importierte Targets kennen ihre abhaengigen Fund-Abhaengigkeiten nicht
target_link_libraries(pplib::core INTERFACE pcre2-8 pcre2-32 bz2 z)

# ==============================================================================
# winmusik4
# ==============================================================================
file(GLOB WMLIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(wmlib STATIC ${WMLIB_SOURCES})
set_target_properties(wmlib PROPERTIES OUTPUT_NAME winmusik4)

target_include_directories(wmlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(wmlib PRIVATE
    -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-function)

target_link_libraries(wmlib PUBLIC pplib7 pplib::core pplib::audio pplib::internet)

if(WMLIB_BUILD_TESTS)
    add_subdirectory(tests)
endif()