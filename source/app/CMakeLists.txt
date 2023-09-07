include("CheckIPOSupported")

check_ipo_supported(RESULT "WANDA_IPO_SUPPORTED")

add_subdirectory("wandac")
add_subdirectory("wandad")