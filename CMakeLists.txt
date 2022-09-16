cmake_minimum_required(VERSION 3.24)

project("wanda"
  LANGUAGES CXX
  VERSION "1.0.0"
)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package("asio")
find_package("Boost")
find_package("JPEG")
find_package("PNG")
find_package("lyra")
find_package("spdlog")
find_package("Threads")

find_package("PkgConfig" REQUIRED)

pkg_check_modules("GIO"
  REQUIRED
  IMPORTED_TARGET
  GLOBAL
  "gio-2.0"
)

pkg_check_modules("libmagic"
  REQUIRED
  IMPORTED_TARGET
  GLOBAL
  "libmagic"
)

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

target_compile_features("${PROJECT_NAME}" PUBLIC
  "cxx_std_20"
)

target_link_libraries("${PROJECT_NAME}" PUBLIC
  "asio::asio"
  "spdlog::spdlog"
  "Threads::Threads"
)

target_include_directories("${PROJECT_NAME}" SYSTEM
  PUBLIC "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties("${PROJECT_NAME}" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
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

target_link_libraries("${PROJECT_NAME}d" PRIVATE
  "${PROJECT_NAME}"
  "bfg::lyra"
  "boost::boost"
  "JPEG::JPEG"
  "PkgConfig::GIO"
  "PkgConfig::libmagic"
  "PNG::PNG"
)

set_target_properties("${PROJECT_NAME}d" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
)

add_executable("${PROJECT_NAME}c"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/commander.cpp"
  "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/wandac.cpp"

  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/commander.hpp"
)

target_link_libraries("${PROJECT_NAME}c" PRIVATE
  "${PROJECT_NAME}"
  "bfg::lyra"
)

set_target_properties("${PROJECT_NAME}c" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
)

# Install Targets

install(TARGETS
  "${PROJECT_NAME}"
  "${PROJECT_NAME}c"
  "${PROJECT_NAME}d"
)
