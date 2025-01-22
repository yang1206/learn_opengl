add_executable(chapter4_app_package
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)

# 链接共享库
target_link_libraries(chapter4_app_package ${SHARED_LIBS} wrapper app)
