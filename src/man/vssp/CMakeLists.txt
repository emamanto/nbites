cmake_minimum_required( VERSION 2.8.3 )

set(VSSP_SRCS
  Objects.cpp
  Scenes.cpp
  MemoryModule.cpp
  )

add_library(
  vssp
  STATIC
  ${VSSP_SRCS} )

target_link_libraries(
  vssp
  portals
  messages
)