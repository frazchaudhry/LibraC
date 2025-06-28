cmake_minimum_required(VERSION 3.31)
project(LibraC C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(src)
add_subdirectory(src/external/GLAD)
add_subdirectory(src/external/glfw)
add_subdirectory(src/windows)
add_subdirectory(src/linux)

add_library(LibraC STATIC src/libraCore.c
        src/libraVideo.c
        src/libraC.h
        src/windows/libraC-windows.h
        src/linux/libraC-linux.h
        src/windows/libraC-windows.c
        src/linux/libraC-linux.c
        src/libraMath.h
        src/libraMath.c)

target_include_directories(LibraC PUBLIC src src/external/glfw/include/GLFW)

target_link_libraries(LibraC PUBLIC glfw glad)

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
