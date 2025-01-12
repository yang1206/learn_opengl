add_executable(chapter4
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter4 ${SHARED_LIBS} wrapper app)
