if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading 'conan' integration")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include("${CMAKE_BINARY_DIR}/conan.cmake")

conan_check(REQUIRED)
conan_add_remote(NAME "bincrafters" URL "https://api.bintray.com/conan/bincrafters/public-conan")
conan_cmake_run(CONANFILE "conanfile.py" BASIC_SETUP CMAKE_TARGETS BUILD "missing" OUTPUT_QUIET)
