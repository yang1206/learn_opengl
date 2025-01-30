add_executable(chapter11_Matrix
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

add_dependencies(chapter11_Matrix copy_resources)

# 链接共享库
target_link_libraries(chapter11_Matrix ${SHARED_LIBS} gl_Framework wrapper app)
