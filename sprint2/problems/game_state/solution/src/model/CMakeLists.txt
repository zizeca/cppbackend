


#set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_MODEL 
  model.cpp  
  model_gses.cpp 
  model_map.cpp 
  tag_invokers.cpp
)
message(${SRCDIR})

set(HEADER_MODEL 
  model_gses.h  
  model.h  
  model_map.h
  dir_vector.h
  tag_invokers.h
)

message(${SRC})

target_sources(${EXECUTE} PUBLIC ${SRC_MODEL} ${HEADER_MODEL})