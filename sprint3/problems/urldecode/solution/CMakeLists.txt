cmake_minimum_required(VERSION 3.13)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)

# обратите внимание на аргумент TARGETS у команды conan_basic_setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(SRCDIR src)
set(TESTDIR tests)

set(EXECUTE boost_tests)

set(SRC 
	${SRCDIR}/url_decode.cpp
  ${SRCDIR}/url_decode.h
)

set(TEST 
	${TESTDIR}/boost_test_main.cpp
)






find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)



add_executable(${EXECUTE} ${SRC} ${TEST})
target_include_directories(${EXECUTE} PRIVATE CONAN_PKG::boost)
# target_link_libraries(${EXECUTE} PRIVATE Threads::Threads CONAN_PKG::boost)
target_link_libraries(${EXECUTE} PRIVATE CONAN_PKG::boost)
