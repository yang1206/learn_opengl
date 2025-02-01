add_executable(chapter12_coordinate
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

add_dependencies(chapter12_coordinate copy_resources)

# 链接共享库
target_link_libraries(chapter12_coordinate ${SHARED_LIBS} gl_Framework wrapper app)
