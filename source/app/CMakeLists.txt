include("CheckIPOSupported")

check_ipo_supported(RESULT "WANDA_IPO_SUPPORTED")

find_package("asio")
find_package("lyra")
find_package("spdlog")

#[=====[ Client ]=====]

add_executable("wandac"
  "src/wandac.cpp"
)

target_link_libraries("${PROJECT_NAME}c" PRIVATE
  "wanda::control"
  "wanda::proto"
  "wanda::system"

  "asio::asio"
  "bfg::lyra"
  "spdlog::spdlog_header_only"
)

set_target_properties("wandac" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandac")

#[=====[ Server ]=====]

add_executable("wandad"
  "src/wandad.cpp"
)

target_link_libraries("wandad" PRIVATE
  "wanda::control"
  "wanda::proto"
  "wanda::std_ext"
  "wanda::system"

  "asio::asio"
  "bfg::lyra"
  "spdlog::spdlog_header_only"
)

set_target_properties("wandad" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandad")
