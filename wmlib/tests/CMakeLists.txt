include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.18.0/googletest-1.18.0.tar.gz
    URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(wmlib_test ${TEST_SOURCES})
target_include_directories(wmlib_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(wmlib_test PRIVATE wmlib gtest)

if(WMLIB_ENABLE_COVERAGE)
    target_compile_options(wmlib_test PRIVATE --coverage -O0 -g)
    target_link_options(wmlib_test PRIVATE --coverage)
endif()

gtest_discover_tests(wmlib_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)