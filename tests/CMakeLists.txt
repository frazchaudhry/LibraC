# In tests/CMakeLists.txt
find_package(GTest CONFIG REQUIRED)
file(GLOB SOURCES "../src/*.*")
file(GLOB WINSOURCES "../src/windows/*.c")
file(GLOB LINUXSOURCES "../src/linux/*.c")
add_executable(LibraCTests libraCoreTests.cpp ${SOURCES} ${WINSOURCES} ${LINUXSOURCES}) # Include relevant C files
target_include_directories(LibraCTests PUBLIC ../src ../src/windows ../src/linux
../external/cglm ../external/GLAD/include/ ../external/stb ../external/debug_break/)
target_link_libraries(LibraCTests GTest::gtest_main SDL3::SDL3 glad -lm) # Link against Google Test

include(GoogleTest)
gtest_discover_tests(LibraCTests)
