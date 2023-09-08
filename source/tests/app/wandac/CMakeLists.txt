add_executable("wanda-tests-wandac"
    "src/cli.cpp"
)

target_link_libraries("wanda-tests-wandac" PRIVATE
    "${PROJECT_NAME}c-components"

    "Catch2::Catch2WithMain"
)

catch_discover_tests("wanda-tests-wandac")