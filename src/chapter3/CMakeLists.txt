add_executable(chapter3_checkError
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter3_checkError ${SHARED_LIBS} wrapper)