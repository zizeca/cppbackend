cmake_minimum_required(VERSION 3.11)

project(HelloAsync CXX)
set(CMAKE_CXX_STANDARD 20)

set(SRCDIR src)

set(EXECUTE hello_async)

set(SRC 
  ${SRCDIR}/main.cpp 
  ${SRCDIR}/Session.cpp
)

set(HEADER 
  ${SRCDIR}/ReportError.hpp
  ${SRCDIR}/ServeHttp.hpp
  ${SRCDIR}/Listener.hpp
  ${SRCDIR}/Session.hpp
)

# auto add cpp and hpp files
#file(GLOB SRC
#     "${SRCDIR}/*.hpp"
#     "${SRCDIR}/*.cpp"
#)


# conan required
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Boost required
find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Под Windows нужно определить макрос NOMINMAX для корректной работы при включении
# библиотеки minisound
if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

add_executable(${EXECUTE} ${SRC} ${HEADER})
target_link_libraries(${EXECUTE} PRIVATE Threads::Threads ${Boost_LIBRARIES})
