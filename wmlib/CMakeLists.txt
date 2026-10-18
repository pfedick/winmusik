cmake_minimum_required(VERSION 3.20)
project(wmlib LANGUAGES CXX)

# pplib8: im Monorepo schon vorhanden, im Solo-Build selbst suchen
if(NOT TARGET pplib::core)
    find_package(pplib8 REQUIRED)
endif()

file(GLOB WMLIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(wmlib STATIC ${WMLIB_SOURCES})
set_target_properties(wmlib PROPERTIES OUTPUT_NAME winmusik4)

target_include_directories(wmlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(wmlib PRIVATE -Wall -Wextra -Wno-unknown-pragmas
                                     -Wno-unused-variable -Wno-unused-function)

target_link_libraries(wmlib PUBLIC pplib::core pplib::audio pplib::internet)

if(WMLIB_BUILD_TESTS)
    add_subdirectory(tests)
endif()

