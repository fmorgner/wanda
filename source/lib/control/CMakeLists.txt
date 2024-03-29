cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM LIB_NAME)

file(GLOB_RECURSE LIB_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.hpp")
file(GLOB_RECURSE LIB_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.cpp")

add_library("wanda-${LIB_NAME}" ${WANDA_LIBRARY_TYPE}
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
    "wanda::proto"
    "wanda::system"

    "Boost::headers"
    "spdlog::spdlog_header_only"
)

if(NOT WANDA_APPLICATIONS_ONLY)
    install(TARGETS "wanda-${LIB_NAME}"
        FILE_SET HEADERS
    )
endif()

add_library("wanda::${LIB_NAME}" ALIAS "wanda-${LIB_NAME}")
