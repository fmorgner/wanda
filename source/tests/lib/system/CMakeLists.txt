add_executable("wanda-tests-system"
    "src/xdg.cpp"
)

target_link_libraries("wanda-tests-system"
    "wanda::system"

    "Catch2::Catch2WithMain"
)

catch_discover_tests("wanda-tests-system")