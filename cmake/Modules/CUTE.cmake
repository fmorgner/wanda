if(NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/cute/cute/cute.h")
  message(STATUS "CUTE: Submodule 'lib/cute' not checked out. Disabling tests.")
  set(CUTE_FOUND OFF)
else()
  message(STATUS "CUTE: Found submodule 'lib/cute'. Enabling tests.")
  include("CTest")
  enable_testing()
  add_library("LIB::CUTE" INTERFACE IMPORTED)
  set_property(TARGET "LIB::CUTE" PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/lib/cute")
  set(CUTE_FOUND ON)

  add_library("${PROJECT_NAME}_test_driver" OBJECT
    "tests/${PROJECT_NAME}/driver.cpp"
  )
  target_include_directories("${PROJECT_NAME}_test_driver" SYSTEM
    PUBLIC "${PROJECT_SOURCE_DIR}/lib/cute"
  )

  function("cute_test" SUITE_NAME)
    if(NOT SUITE_NAME)
      message(FATAL_ERROR "Missing argument 'SUITE_NAME'!")
    endif()

    add_executable("${PROJECT_NAME}_${SUITE_NAME}_tests"
      "tests/${PROJECT_NAME}/test_suite_${SUITE_NAME}.cpp"
    )
    target_link_libraries("${PROJECT_NAME}_${SUITE_NAME}_tests"
      "${PROJECT_NAME}_test_driver"
      "${PROJECT_NAME}"
    )
    add_test(NAME "${SUITE_NAME}_tests"
      COMMAND "${PROJECT_NAME}_${SUITE_NAME}_tests"
    )
  endfunction()
endif()