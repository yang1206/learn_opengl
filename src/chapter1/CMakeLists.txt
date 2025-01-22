add_executable(chapter1_window
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter1_window ${SHARED_LIBS})