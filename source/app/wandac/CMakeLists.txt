add_executable("wandac"
  "src/cli.cpp"
  "src/listener.cpp"
  "src/main.cpp"
)

target_include_directories("${PROJECT_NAME}c" PRIVATE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries("${PROJECT_NAME}c" PRIVATE
  "wanda::control"
  "wanda::proto"
  "wanda::system"

  "bfg::lyra"
  "spdlog::spdlog_header_only"
)

set_target_properties("wandac" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandac")
