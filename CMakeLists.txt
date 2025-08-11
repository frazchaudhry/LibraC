cmake_minimum_required(VERSION 3.15...4.0)
project(LibraC C CXX)

set(CMAKE_C_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# In your main CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0 # Or a specific commit/branch
)
FetchContent_MakeAvailable(googletest)
include(CTest)

enable_testing()

add_subdirectory(src)
add_subdirectory(external/GLAD)
add_subdirectory(external/glfw)
add_subdirectory(src/windows)
add_subdirectory(src/linux)
add_subdirectory(tests)

add_library(LibraC STATIC src/libraCore.c
    src/libraVideo.c
    src/libraC.h
    src/windows/libraC-windows.h
    src/linux/libraC-linux.h
    src/windows/libraC-windows.c
    src/linux/libraC-linux.c
    src/libraMath.h
    src/libraMath.c)

target_include_directories(LibraC PUBLIC src external external/glfw/include/GLFW external/cglm)

target_link_libraries(LibraC PUBLIC glfw glad)

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
