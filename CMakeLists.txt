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

add_executable("wanda"
  "src/main.cpp"
  "src/setting.cpp"
  "src/setting.hpp"
  "src/variant.cpp"
  "src/variant.hpp"
  )
target_link_libraries("wanda"
  "CONAN_PKG::boost_config"
  "SYSTEM::GIO"
  )
