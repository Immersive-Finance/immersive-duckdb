include_directories(../../immersive)


# Create the object library for unit tests
add_library_unity(
        test_immersive
        OBJECT
        test_calculation_table.cpp
        ../../immersive/BlackScholes.cpp
        ../../immersive/CalculationTable.cpp
)

# List of all object files
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:test_immersive>
        PARENT_SCOPE
)

