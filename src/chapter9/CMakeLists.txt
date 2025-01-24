add_executable(chapter9_Texture
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)


add_dependencies(chapter9_Texture copy_resources)

# 链接共享库
target_link_libraries(chapter9_Texture ${SHARED_LIBS} wrapper app)
