add_executable(chapter3
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter3 ${SHARED_LIBS} wrapper)