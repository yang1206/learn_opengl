add_executable(chapter6_EBO
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter6_EBO ${SHARED_LIBS} wrapper app)
