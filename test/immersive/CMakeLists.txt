include_directories(../../immersive)

add_library_unity(
  test_immersive
  OBJECT
  test_calculation_table.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:test_immersive>
    PARENT_SCOPE)
