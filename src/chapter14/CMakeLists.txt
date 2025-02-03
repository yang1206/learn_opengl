add_executable(chapter14_camera2
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

add_dependencies(chapter14_camera2 copy_resources)

# 链接共享库
target_link_libraries(chapter14_camera2 ${SHARED_LIBS} gl_Framework wrapper app)
