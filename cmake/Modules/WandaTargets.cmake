include(CMakeParseArguments)

function(add_wanda_library)
  set(OPTIONS)
  set(SV_ARGS "NAME")
  set(MV_ARGS "SOURCES;HEADERS;LIBRARIES")
  cmake_parse_arguments(WANDA_LIBRARY "${OPTIONS}" "${SV_ARGS}" "${MV_ARGS}" ${ARGN})

  if(NOT WANDA_LIBRARY_NAME)
    message(FATAL_ERROR "Missing argument 'NAME' to call of 'add_wanda_library'")
  endif()

  if(NOT WANDA_LIBRARY_SOURCES)
    message(FATAL_ERROR "Missing argument 'SOURCES' to call of 'add_wanda_library'")
  endif()

  set(RESOLVED_SOURCES)
  foreach(SOURCE IN LISTS WANDA_LIBRARY_SOURCES)
    list(APPEND RESOLVED_SOURCES "src/wanda/${SOURCE}")
  endforeach()

  set(RESOLVED_HEADERS)
  foreach(HEADER IN LISTS WANDA_LIBRARY_HEADERS)
    list(APPEND RESOLVED_HEADERS "include/wanda/${HEADER}")
  endforeach()

  add_library("${WANDA_LIBRARY_NAME}"
    ${RESOLVED_SOURCES} # Source files
    ${RESOLVED_HEADERS} # Header files
  )

  target_include_directories("${WANDA_LIBRARY_NAME}" SYSTEM PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )

  target_link_libraries("${WANDA_LIBRARY_NAME}"
    ${WANDA_LIBRARY_LIBRARIES}
  )

  set_target_properties("${WANDA_LIBRARY_NAME}"
    PROPERTIES PUBLIC_HEADER "${RESOLVED_HEADERS}"
  )

  install(TARGETS "${WANDA_LIBRARY_NAME}"
    ARCHIVE DESTINATION "lib" PUBLIC_HEADER DESTINATION "include/wanda"
  )
endfunction()

function(add_wanda_executable)
  set(OPTIONS)
  set(SV_ARGS "NAME")
  set(MV_ARGS "SOURCES;HEADERS;LIBRARIES")
  cmake_parse_arguments(WANDA_EXECUTABLE "${OPTIONS}" "${SV_ARGS}" "${MV_ARGS}" ${ARGN})

  if(NOT WANDA_EXECUTABLE_NAME)
    message(FATAL_ERROR "Missing argument 'NAME' to call of 'add_wanda_executable'")
  endif()

  if(NOT WANDA_EXECUTABLE_SOURCES)
    message(FATAL_ERROR "Missing argument 'SOURCES' to call of 'add_wanda_executable'")
  endif()

  set(RESOLVED_SOURCES)
  foreach(SOURCE IN LISTS WANDA_EXECUTABLE_SOURCES)
    list(APPEND RESOLVED_SOURCES "src/wanda/${SOURCE}")
  endforeach()

  set(RESOLVED_HEADERS)
  foreach(HEADER IN LISTS WANDA_EXECUTABLE_HEADERS)
    list(APPEND RESOLVED_HEADERS "include/wanda/${HEADER}")
  endforeach()

  add_executable("${WANDA_EXECUTABLE_NAME}"
    ${RESOLVED_SOURCES} # Source files
    ${RESOLVED_HEADERS} # Header files
  )

  target_include_directories("${WANDA_EXECUTABLE_NAME}" SYSTEM PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  )

  target_link_libraries("${WANDA_EXECUTABLE_NAME}"
    ${WANDA_EXECUTABLE_LIBRARIES}
  )

  install(TARGETS "${WANDA_EXECUTABLE_NAME}"
    ARCHIVE DESTINATION "lib" RUNTIME DESTINATION "bin"
  )
endfunction()

function(add_wanda_test)
  set(OPTIONS)
  set(SV_ARGS "NAME")
  set(MV_ARGS "HEADERS;LIBRARIES")
  cmake_parse_arguments(WANDA_TEST "${OPTIONS}" "${SV_ARGS}" "${MV_ARGS}" ${ARGN})

  if(NOT WANDA_TEST_NAME)
    message(FATAL_ERROR "Missing argument 'NAME' to call of 'add_wanda_test'")
  endif()

  if(NOT CUTE_FOUND)
    message(STATUS "CUTE not found. Skipping test '${WANDA_TEST_NAME}'")
    return()
  endif()

  set(RESOLVED_HEADERS)
  foreach(HEADER IN LISTS WANDA_TEST_HEADERS)
    list(APPEND RESOLVED_HEADERS "include/wanda/${HEADER}")
  endforeach()

  if(NOT TARGET "wanda_test_driver")
    add_library("wanda_test_driver" OBJECT "tests/wanda/driver.cpp")
    target_link_libraries("wanda_test_driver" "LIB::CUTE")
  endif()

  add_executable("test_${WANDA_TEST_NAME}"
    "tests/wanda/test_suite_${WANDA_TEST_NAME}.cpp"
    ${RESOLVED_HEADERS} # Header files
    $<TARGET_OBJECTS:wanda_test_driver>
  )

  target_include_directories("test_${WANDA_TEST_NAME}" SYSTEM PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  )

  target_link_libraries("test_${WANDA_TEST_NAME}"
    "LIB::CUTE"
    ${WANDA_TEST_LIBRARIES}
  )

  add_test(NAME "${WANDA_TEST_NAME}" COMMAND "test_${WANDA_TEST_NAME}")
endfunction()