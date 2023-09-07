add_executable("wandad"
  "src/main.cpp"
)

target_link_libraries("wandad" PRIVATE
  "wanda::control"
  "wanda::proto"
  "wanda::std_ext"
  "wanda::system"

  "bfg::lyra"
  "spdlog::spdlog_header_only"
)

set_target_properties("wandad" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandad")
