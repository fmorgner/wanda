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

include("ConanDependencies")
include("SystemDependencies")
include("CUTE")
include("WandaTargets")

add_wanda_library(NAME "wanda"
  SOURCES
  "command.cpp"
  "control_connection.cpp"
  "environment.cpp"
  "logging.cpp"
  "message.cpp"
  "xdg.cpp"

  HEADERS
  "command.hpp"
  "control_connection.hpp"
  "deferred_failure.hpp"
  "environment.hpp"
  "expected.hpp"
  "keyed.hpp"
  "logging.hpp"
  "message.hpp"
  "optional.hpp"
  "type_wrapper.hpp"
  "xdg.hpp"

  LIBRARIES
  "CONAN_PKG::asio"
  "CONAN_PKG::spdlog"
  "CONAN_PKG::range-v3"
  "SYSTEM::C++FS"
  "Threads::Threads"
)

add_wanda_executable(NAME "wandad"
  SOURCES
  "control_interface.cpp"
  "filesystem.cpp"
  "setting.cpp"
  "wallpaper.cpp"
  "wandad.cpp"

  HEADERS
  "control_interface.hpp"
  "filesystem.hpp"
  "setting.hpp"
  "wallpaper.hpp"

  LIBRARIES
  "wanda"
  "CONAN_PKG::clara"
  "SYSTEM::GIO"
)

add_wanda_executable(NAME "wandac"
  SOURCES
  "commander.cpp"
  "wandac.cpp"

  HEADERS
  "commander.hpp"

  LIBRARIES
  "wanda"
  "CONAN_PKG::clara"
)

add_wanda_test(NAME "xdg"
  LIBRARIES
  "wanda"
)