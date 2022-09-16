find_package("asio")
find_package("Boost")
find_package("JPEG")
find_package("PNG")
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

file(GLOB_RECURSE WANDA_LIB_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.cpp")
file(GLOB_RECURSE WANDA_LIB_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.hpp")

add_library("wanda")

target_sources("wanda" PRIVATE
    ${WANDA_LIB_SOURCES}
)

target_sources("wanda" INTERFACE
    FILE_SET HEADERS
    FILES ${WANDA_LIB_HEADERS}
    BASE_DIRS "include"
)

target_include_directories("wanda" PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories("wanda" SYSTEM PUBLIC
    $<INSTALL_INTERFACE:include>
)

target_compile_features("wanda" PUBLIC
    "cxx_std_20"
)

target_link_libraries("wanda" PUBLIC
    "asio::asio"
    "boost::boost"
    "JPEG::JPEG"
    "PkgConfig::GIO"
    "PkgConfig::libmagic"
    "PNG::PNG"
    "spdlog::spdlog"
    "Threads::Threads"
)

install(TARGETS "wanda"
    FILE_SET HEADERS
)

add_library("wanda::wanda" ALIAS "wanda")
