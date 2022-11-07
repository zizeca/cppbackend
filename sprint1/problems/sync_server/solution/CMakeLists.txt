cmake_minimum_required(VERSION 3.11)

project(Hello CXX)
set(CMAKE_CXX_STANDARD 20)
set(EXEC hello)
find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# file(GLOB SRC  "${CMAKE_SOURCE_DIR}/src/*.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


set(SRC src/main.cpp)
add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)

add_executable(${EXEC} ${SRC})
target_link_libraries(${EXEC} PRIVATE Threads::Threads)
