cmake_minimum_required(VERSION 3.20)
project(WinMusik LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)              # gnu++20, wie bisher
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------- Aufbau ----------------
option(WINMUSIK_BUILD_GUI   "GUI (Qt6) mitbauen"        OFF)
option(WINMUSIK_BUILD_TESTS "Unit-Tests fuer wmlib"     OFF)
option(WMLIB_ENABLE_COVERAGE "Code-Coverage fuer wmlib" OFF)

# ---------------- pplib8 als Submodul einbinden ----------------
set(PPLIB_ENABLE_CORE     ON  CACHE BOOL "" FORCE)
set(PPLIB_ENABLE_AUDIO    ON  CACHE BOOL "" FORCE)
set(PPLIB_ENABLE_INTERNET ON  CACHE BOOL "" FORCE)
set(PPLIB_ENABLE_CRYPTO   OFF CACHE BOOL "" FORCE)
set(PPLIB_ENABLE_GRAFIX   OFF CACHE BOOL "" FORCE)
set(PPLIB_ENABLE_DATABASE OFF CACHE BOOL "" FORCE)
set(PPLIB_ENABLE_PICO     OFF CACHE BOOL "" FORCE)
set(PPLIB_BUILD_TESTS     OFF CACHE BOOL "" FORCE)   # laufen im pplib8-Projekt selbst
set(PPLIB_ENABLE_COVERAGE ${WMLIB_ENABLE_COVERAGE} CACHE BOOL "" FORCE)

add_subdirectory(pplib8)
add_subdirectory(wmlib)

if(WINMUSIK_BUILD_GUI)
    add_subdirectory(gui)
endif()

