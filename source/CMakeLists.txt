cmake_minimum_required(VERSION "3.27")

project("wanda"
  LANGUAGES CXX
  VERSION "1.0.0"
)

find_package("Boost" REQUIRED COMPONENTS "headers")
find_package("Catch2" REQUIRED COMPONENTS "Catch2WithMain")
find_package("JPEG" REQUIRED)
find_package("lyra" REQUIRED)
find_package("spdlog" REQUIRED)
find_package("PNG" REQUIRED)

include("CTest")
include("Catch")

add_subdirectory("app")
add_subdirectory("lib")
add_subdirectory("tests")
