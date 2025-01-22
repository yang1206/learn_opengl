add_executable(chapter2_events
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter2_events ${SHARED_LIBS})