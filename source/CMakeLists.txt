cmake_minimum_required(VERSION "3.27")

project("wanda"
  LANGUAGES CXX
  VERSION "1.0.0"
)

enable_testing()

add_subdirectory("app")
add_subdirectory("lib")
add_subdirectory("tests")