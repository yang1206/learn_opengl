add_executable(chapter8_SHADER
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

add_dependencies(chapter8_SHADER copy_resources)


# 链接共享库
target_link_libraries(chapter8_SHADER ${SHARED_LIBS} wrapper app)
