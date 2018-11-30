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

include("ConanDependencies")
include("SystemDependencies")

add_library("core" OBJECT
  "src/control_connection.cpp"
  "src/control_connection.hpp"
  "src/control_interface.cpp"
  "src/control_interface.hpp"
  "src/environment.cpp"
  "src/environment.hpp"
  "src/deferred_failure.hpp"
  "src/keyed.hpp"
  "src/optional.hpp"
  "src/type_wrapper.hpp"
  "src/xdg.cpp"
  "src/xdg.hpp"
)

target_link_libraries("core"
  "CONAN_PKG::boost_asio"
  "CONAN_PKG::boost_program_options"
  "SYSTEM::C++FS"
  "Threads::Threads"
)

add_executable("wandad"
  "src/filesystem.cpp"
  "src/filesystem.hpp"
  "src/setting.cpp"
  "src/setting.hpp"
  "src/wallpaper.cpp"
  "src/wallpaper.hpp"
  "src/wandad.cpp"
  )

target_link_libraries("wandad"
  "core"
  "SYSTEM::GIO"
  )

add_executable("wandac"
  "src/wandac.cpp"
)

target_link_libraries("wandac"
  "core"
)