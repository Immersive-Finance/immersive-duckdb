# Specify the Python version


# Download pybind11
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        stable
)
FetchContent_MakeAvailable(pybind11)

# Set path definitions (keep your definitions here)
add_definitions(-DDUCKDB_ROOT_DIRECTORY="${PROJECT_SOURCE_DIR}")

# Define all source files that should be compiled for each target
set(SOURCE_FILES immersive.cpp CalculationTable.cpp ${ALL_OBJECT_FILES})

# Create the Python extension module
pybind11_add_module(immersive MODULE ${SOURCE_FILES})

# Create a static library
add_library(immersive_static STATIC ${SOURCE_FILES})

# Link with duckdb_static (and other necessary libraries)
target_link_libraries(immersive PRIVATE duckdb_static)

