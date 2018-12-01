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

##### Wanda CORE #####

set(WANDA_CORE_SOURCES
  "src/control_connection.cpp"
  "src/environment.cpp"
  "src/message.cpp"
  "src/xdg.cpp"
)

set(WANDA_CORE_HEADERS
  "src/control_connection.hpp"
  "src/deferred_failure.hpp"
  "src/environment.hpp"
  "src/keyed.hpp"
  "src/message.hpp"
  "src/optional.hpp"
  "src/type_wrapper.hpp"
  "src/xdg.hpp"
)

set(WANDA_CORE_LIBRARIES
  "CONAN_PKG::asio"
  "CONAN_PKG::spdlog"
  "SYSTEM::C++FS"
  "Threads::Threads"
)

add_library("core" OBJECT ${WANDA_CORE_SOURCES} ${WANDA_CORE_HEADERS})
target_link_libraries("core" ${WANDA_CORE_LIBRARIES})

##### Wanda Daemon #####

set(WANDA_DAEMON_SOURCES
  "src/control_interface.cpp"
  "src/filesystem.cpp"
  "src/setting.cpp"
  "src/wallpaper.cpp"
  "src/wandad.cpp"
)

set(WANDA_DAEMON_HEADERS
  "src/control_interface.hpp"
  "src/filesystem.hpp"
  "src/setting.hpp"
  "src/wallpaper.hpp"
)

set(WANDA_DAEMON_LIBRARIES
  "core"
  "SYSTEM::GIO"
)

add_executable("wandad" ${WANDA_DAEMON_SOURCES} ${WANDA_DAEMON_HEADERS})
target_link_libraries("wandad" ${WANDA_DAEMON_LIBRARIES})

##### Wanda Controller #####

set(WANDA_CONTROLLER_SOURCES
  "src/commander.cpp"
  "src/wandac.cpp"
)

set(WANDA_CONTROLLER_HEADERS
  "src/commander.hpp"
)

set(WANDA_CONTROLLER_LIBRARIES
  "core"
)

add_executable("wandac" ${WANDA_CONTROLLER_SOURCES} ${WANDA_CONTROLLER_HEADERS})
target_link_libraries("wandac" ${WANDA_CONTROLLER_LIBRARIES})