cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM LIB_NAME)

file(GLOB_RECURSE LIB_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "**/*.hpp")

add_library("wanda-${LIB_NAME}" INTERFACE)

target_sources("wanda-${LIB_NAME}" INTERFACE
    FILE_SET HEADERS
    FILES ${LIB_HEADERS}
    BASE_DIRS "include"
)

target_include_directories("wanda-${LIB_NAME}" INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_include_directories("wanda-${LIB_NAME}" SYSTEM INTERFACE
    "$<INSTALL_INTERFACE:include>"
)

target_compile_features("wanda-${LIB_NAME}" INTERFACE
    "cxx_std_20"
)

if(NOT WANDA_APPLICATIONS_ONLY)
    install(TARGETS "wanda-${LIB_NAME}"
        FILE_SET HEADERS
    )
endif()

add_library("wanda::${LIB_NAME}" ALIAS "wanda-${LIB_NAME}")
