cmake_minimum_required(VERSION 3.10)
project("wanda" LANGUAGES CXX)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD "17")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

enable_testing()

# Conan Dependencies

include("Conan")

conan_add_remote(NAME "fmorgner"
  URL "https://api.bintray.com/conan/fmorgner/conan-public"
)
conan_cmake_run(CONANFILE "conanfile.py"
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD "missing"
  OUTPUT_QUIET
)

# System Dependencies

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package("Threads")
find_package("PkgConfig" REQUIRED)
pkg_check_modules("GIO"
  REQUIRED
  IMPORTED_TARGET
  GLOBAL
  "gio-2.0"
)

find_library(C++FS_LIBRARIES
  "stdc++fs"
  DOC "The C++ Standard Library Filesystem implementation"
)
if(NOT C++FS_LIBRARIES)
  message(FATAL_ERROR "Failed to find libstdc++fs")
endif()

# Core Library

add_library("${PROJECT_NAME}" STATIC
  "${PROJECT_SOURCE_DIR}/src/wanda/command.cpp"
  "${PROJECT_SOURCE_DIR}/src/wanda/control_connection.cpp"
  "${PROJECT_SOURCE_DIR}/src/wanda/environment.cpp"
  "${PROJECT_SOURCE_DIR}/src/wanda/logging.cpp"
  "${PROJECT_SOURCE_DIR}/src/wanda/message.cpp"
  "${PROJECT_SOURCE_DIR}/src/wanda/xdg.cpp"

  "${PROJECT_SOURCE_DIR}/include/wanda/command.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/control_connection.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/deferred_failure.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/environment.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/expected.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/keyed.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/logging.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/message.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/optional.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/type_wrapper.hpp"
  "${PROJECT_SOURCE_DIR}/include/wanda/xdg.hpp"
)
target_link_libraries("${PROJECT_NAME}"
  "${C++FS_LIBRARIES}"
  "CONAN_PKG::asio"
  "CONAN_PKG::range-v3"
  "CONAN_PKG::spdlog"
  "Threads::Threads"
)
target_include_directories("${PROJECT_NAME}" SYSTEM
  PUBLIC "${PROJECT_SOURCE_DIR}/include"
)

# Core Executables

add_executable("${PROJECT_NAME}d"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/control_interface.cpp"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/filesystem.cpp"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/setting.cpp"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/wallpaper.cpp"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/wandad.cpp"

  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/control_interface.hpp"
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/filesystem.hpp"
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/setting.hpp"
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/wallpaper.hpp"
)
target_link_libraries("${PROJECT_NAME}d"
  "${PROJECT_NAME}"
  "CONAN_PKG::lyra"
  "PkgConfig::GIO"
)

add_executable("${PROJECT_NAME}c"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/commander.cpp"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/wandac.cpp"

  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/commander.hpp"
)
target_link_libraries("${PROJECT_NAME}c"
  "${PROJECT_NAME}"
  "CONAN_PKG::lyra"
)

# Automated Tests

add_library("${PROJECT_NAME}_test_driver" OBJECT
  "tests/${PROJECT_NAME}/driver.cpp"
)
target_link_libraries("${PROJECT_NAME}_test_driver"
  "CONAN_PKG::CUTE"
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

cute_test("xdg")

# Install Targets

install(TARGETS "${PROJECT_NAME}" "${PROJECT_NAME}c" "${PROJECT_NAME}d")
