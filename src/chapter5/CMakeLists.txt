add_executable(chapter5
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter5 ${SHARED_LIBS} wrapper app)
