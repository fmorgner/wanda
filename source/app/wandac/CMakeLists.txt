add_library("${PROJECT_NAME}c-components" OBJECT
  "src/cli.cpp"
  "src/listener.cpp"
)

target_include_directories("${PROJECT_NAME}c-components" PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries("${PROJECT_NAME}c-components" PUBLIC
  "wanda::control"
  "wanda::proto"

  "bfg::lyra"
  "Boost::headers"
)

add_executable("wandac"
  "src/main.cpp"
)


target_link_libraries("${PROJECT_NAME}c" PRIVATE
  "${PROJECT_NAME}c-components"

  "wanda::system"

  "spdlog::spdlog_header_only"
)

set_target_properties("wandac" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandac")
