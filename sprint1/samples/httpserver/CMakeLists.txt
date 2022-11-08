cmake_minimum_required(VERSION 3.11)

set(SRC src/main.cpp)
set(EXECUTE http_server)

# Проект называется TCP_UDP_Practice и написан на C++
project(TCP_UDP_Practice CXX)
# Исходый код будет компилироваться с поддержкой стандарта С++ 20
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()



add_executable(${EXECUTE} ${SRC})


# Просим компоновщик подключить библиотеку для поддержки потоков
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${EXECUTE} PRIVATE Threads::Threads)