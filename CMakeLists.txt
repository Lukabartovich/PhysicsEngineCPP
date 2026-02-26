cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

# Use C++20 for modern features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find the SDL3 packages
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES})

# 3. "Import" the libraries (Links and Includes in one go)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
