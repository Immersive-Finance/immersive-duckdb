# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)

# Project's name
project(immersive)

# Set path definitions
add_definitions(-DDUCKDB_ROOT_DIRECTORY="${PROJECT_SOURCE_DIR}")

# Add executable target and its source files
# Automatically include all .cpp files
file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
)

# Add include directories
include_directories("./")  # Replace with your actual path


# Add executable target and its source files
add_executable(immersive ${SOURCE_FILES})

# Link libraries
target_link_libraries(immersive PRIVATE duckdb_static)
