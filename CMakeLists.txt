cmake_minimum_required(VERSION 3.15...4.0)
project(LibraC C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# In your main CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0 # Or a specific commit/branch
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if (WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()
FetchContent_MakeAvailable(googletest)
include(CTest)

enable_testing()

add_subdirectory(src)
add_subdirectory(external/GLAD)
add_subdirectory(external/SDL)
add_subdirectory(src/windows)
add_subdirectory(src/linux)
# add_subdirectory(tests)

add_library(LibraC STATIC src/libraCore.c
    src/libraVideo.c
    src/libraC.h
    src/windows/libraC-windows.h
    src/linux/libraC-linux.h
    src/windows/libraC-windows.c
    src/linux/libraC-linux.c
    src/libraMath.h
    src/libraMath.c)
add_executable(LibraCTests tests/libraCoreTests.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wfatal-errors -Wunused-variable)

target_include_directories(${PROJECT_NAME} PUBLIC src external/cglm external/stb external/debug_break)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3 glad -lm )
target_link_libraries(LibraCTests PRIVATE LibraC GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(LibraCTests)

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
