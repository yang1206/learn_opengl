add_executable(chapter10_Texture_Unit
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)


add_dependencies(chapter10_Texture_Unit copy_resources)

# 链接共享库
target_link_libraries(chapter10_Texture_Unit ${SHARED_LIBS} wrapper app)
