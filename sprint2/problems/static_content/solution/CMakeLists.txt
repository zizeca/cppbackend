cmake_minimum_required(VERSION 3.23)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)

# conan gen
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SRCDIR src)

set(EXECUTE game_server)

set(SRC 
	${SRCDIR}/main.cpp
	${SRCDIR}/model.cpp
	${SRCDIR}/boost_json.cpp
	${SRCDIR}/json_loader.cpp
	#${SRCDIR}/request_handler.cpp
	${SRCDIR}/tag_invokers.cpp
  ${SRCDIR}/util.cpp
)

set(HEADER 
  ${SRCDIR}/http_server.h
  ${SRCDIR}/sdk.h
  ${SRCDIR}/model.h
  ${SRCDIR}/tagged.h
  ${SRCDIR}/json_loader.h
  ${SRCDIR}/request_handler.h
  ${SRCDIR}/tag_invokers.h
  ${SRCDIR}/content_type.hpp
  ${SRCDIR}/util.h
  ${SRCDIR}/ApiResponseHandler.hpp
)




find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



add_executable(${EXECUTE} ${SRC} ${HEADER})
target_link_libraries(${EXECUTE} PRIVATE Threads::Threads)
add_subdirectory(${SRCDIR}/http_server)
