cmake_minimum_required(VERSION 3.24)

project("wanda"
  LANGUAGES CXX
  VERSION "1.0.0"
)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_subdirectory("app")
add_subdirectory("lib")
