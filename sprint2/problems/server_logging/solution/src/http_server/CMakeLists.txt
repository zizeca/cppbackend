


#set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC 
  Session.cpp
)
message(${SRCDIR})

set(HEADER 
  ReportError.hpp
  Listener.hpp
  Session.hpp
  ServeHttp.hpp
)

message(${SRC})

target_sources(${EXECUTE} PUBLIC ${SRC} ${HEADER})