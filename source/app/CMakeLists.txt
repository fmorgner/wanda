include("CheckIPOSupported")

check_ipo_supported(RESULT "WANDA_IPO_SUPPORTED")

find_package("lyra")

#[=====[ Server ]=====]

add_executable("wandad"
  "src/wandad.cpp"
)

target_link_libraries("wandad" PRIVATE
  "bfg::lyra"
  "wanda::wanda"
)

set_target_properties("wandad" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandad")

#[=====[ Client ]=====]

add_executable("wandac"
  "src/wandac.cpp"
)

target_link_libraries("${PROJECT_NAME}c" PRIVATE
  "bfg::lyra"
  "wanda::wanda"
)

set_target_properties("wandac" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ${WANDA_IPO_SUPPORTED}
)

install(TARGETS "wandac")