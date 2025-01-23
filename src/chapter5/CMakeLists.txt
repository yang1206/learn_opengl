add_executable(chapter5_VAO_VBO
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter5_VAO_VBO ${SHARED_LIBS} wrapper app)
