if(WANDA_APPLICATIONS_ONLY)
  set(WANDA_LIBRARY_TYPE OBJECT)
endif()

add_subdirectory("control")
add_subdirectory("meta")
add_subdirectory("proto")
add_subdirectory("std_ext")
add_subdirectory("system")
