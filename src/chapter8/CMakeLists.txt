add_executable(chapter8_SHADER
  main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../glad.c
)


set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_BINARY_DIR}/shaders") 
file(COPY ${SHADER_SOURCE_DIR}/ DESTINATION ${SHADER_BINARY_DIR})

# 链接共享库
target_link_libraries(chapter8_SHADER ${SHARED_LIBS} wrapper app)
