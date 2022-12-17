
#set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_MODEL 
  dog.cpp
  game.cpp
  game_session.cpp
  loot_type.cpp
  model_map.cpp
  player.cpp
  player_list.cpp
  tag_invokers.cpp
  token_generator.cpp
)
message(${SRCDIR})

set(HEADER_MODEL 
  building.h
  dog.h
  game.h
  game_session.h
  geometry.h
  loot_type.h
  model_map.h
  office.h
  player.h
  player_list.h
  road.h
  tag_invokers.h
  token_generator.h
)

message(${SRC})

target_sources(${EXECUTE} PUBLIC ${SRC_MODEL} ${HEADER_MODEL})