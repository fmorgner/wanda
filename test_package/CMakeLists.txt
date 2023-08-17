cmake_minimum_required(VERSION "3.27")

project("TestPackage"
    LANGUAGES CXX
    DESCRIPTION "Conan test package for 'Wanda!'"
)

find_package("wanda" REQUIRED
    COMPONENTS
    "control"
    "meta"
    "proto"
    "std_ext"
    "system"
)

add_executable("test"
    "test.cpp"
)

target_link_libraries("test"
    "wanda::control"
    "wanda::meta"
    "wanda::proto"
    "wanda::std_ext"
    "wanda::system"
)
