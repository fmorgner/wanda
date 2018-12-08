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
include("CUTE")

##### Wanda CORE #####

set(WANDA_CORE_SOURCES
  "src/command.cpp"
  "src/control_connection.cpp"
  "src/environment.cpp"
  "src/logging.cpp"
  "src/message.cpp"
  "src/xdg.cpp"
)

set(WANDA_CORE_HEADERS
  "src/command.hpp"
  "src/control_connection.hpp"
  "src/deferred_failure.hpp"
  "src/environment.hpp"
  "src/expected.hpp"
  "src/keyed.hpp"
  "src/logging.hpp"
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

add_library("wanda" ${WANDA_CORE_SOURCES} ${WANDA_CORE_HEADERS})
target_link_libraries("wanda" ${WANDA_CORE_LIBRARIES})
target_include_directories("wanda" INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)
set_target_properties("wanda" PROPERTIES PUBLIC_HEADER "${WANDA_CORE_HEADERS}")
install(TARGETS "wanda" ARCHIVE DESTINATION "lib" PUBLIC_HEADER DESTINATION "include")

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
  "wanda"
  "CONAN_PKG::clara"
  "SYSTEM::GIO"
)

add_executable("wandad" ${WANDA_DAEMON_SOURCES} ${WANDA_DAEMON_HEADERS})
target_link_libraries("wandad" ${WANDA_DAEMON_LIBRARIES})
install(TARGETS "wandad" RUNTIME DESTINATION "bin")

##### Wanda Controller #####

set(WANDA_CONTROLLER_SOURCES
  "src/commander.cpp"
  "src/wandac.cpp"
)

set(WANDA_CONTROLLER_HEADERS
  "src/commander.hpp"
)

set(WANDA_CONTROLLER_LIBRARIES
  "wanda"
  "CONAN_PKG::clara"
)

add_executable("wandac" ${WANDA_CONTROLLER_SOURCES} ${WANDA_CONTROLLER_HEADERS})
target_link_libraries("wandac" ${WANDA_CONTROLLER_LIBRARIES})
install(TARGETS "wandac" RUNTIME DESTINATION "bin")

##### Wanda Core Tests #####
if(CUTE_FOUND)
  set(WANDA_CORE_TESTS_SUITE_SOURCES
    "tests/test_suite_xdg.cpp"
    )

  set(WANDA_CORE_TESTS_SUITE_HEADERS
    "tests/test_suite_xdg.hpp"
  )

  set(WANDA_CORE_TESTS_LIBRARIES
    "wanda"
    "LIB::CUTE"
  )

  add_executable("wanda_core_tests" "tests/core_driver.cpp" ${WANDA_CORE_TESTS_SUITE_SOURCES} ${WANDA_CORE_TESTS_SUITE_HEADERS})
  target_link_libraries("wanda_core_tests" ${WANDA_CORE_TESTS_LIBRARIES})
  add_test(NAME "wanda_core_tests" COMMAND "wanda_core_tests")
endif()