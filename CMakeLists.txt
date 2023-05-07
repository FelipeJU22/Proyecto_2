
set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Proyecto_2)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED


add_executable(Proyecto_2 main.cpp)
#set(raylib_VERBOSE 1)
target_link_libraries(Proyecto_2 raylib)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()