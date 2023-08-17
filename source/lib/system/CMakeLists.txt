cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM LIB_NAME)

file(GLOB_RECURSE LIB_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.hpp")
file(GLOB_RECURSE LIB_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.cpp")

find_package("Boost" REQUIRED)
find_package("fmt" REQUIRED)
find_package("JPEG" REQUIRED)
find_package("PNG" REQUIRED)
find_package("spdlog" REQUIRED)
find_package("glib" REQUIRED)

find_package("PkgConfig" REQUIRED)

pkg_check_modules("libmagic"
  REQUIRED
  IMPORTED_TARGET
  GLOBAL
  "libmagic"
)

add_library("wanda-${LIB_NAME}"
    ${LIB_SOURCES}
)

target_sources("wanda-${LIB_NAME}" INTERFACE
    FILE_SET HEADERS
    FILES ${LIB_HEADERS}
    BASE_DIRS "include"
)

target_include_directories("wanda-${LIB_NAME}" PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_include_directories("wanda-${LIB_NAME}" SYSTEM PUBLIC
    "$<INSTALL_INTERFACE:include>"
)

target_compile_features("wanda-${LIB_NAME}" PUBLIC
    "cxx_std_20"
)

target_link_libraries("wanda-${LIB_NAME}" PUBLIC
    "wanda::meta"
    "wanda::std_ext"

    "boost::boost"
    "fmt::fmt"
    "glib::gio-2.0"
    "JPEG::JPEG"
    "PNG::PNG"
    "spdlog::spdlog"

    "PkgConfig::libmagic"
)

install(TARGETS "wanda-${LIB_NAME}"
    FILE_SET HEADERS
)

add_library("wanda::${LIB_NAME}" ALIAS "wanda-${LIB_NAME}")
