add_executable(chapter13_camera
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

add_dependencies(chapter13_camera copy_resources)

# 链接共享库
target_link_libraries(chapter13_camera ${SHARED_LIBS} gl_Framework wrapper app)
