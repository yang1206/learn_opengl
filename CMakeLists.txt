cmake_minimum_required(VERSION 3.30)
project(glfw_temp)

add_definitions(-DDEBUG)
set(CMAKE_CXX_STANDARD 20)

# 包含头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/include)

# 添加wrapper头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/wrapper)

# 添加app头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application)

# 链接库文件路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib)

# 添加可执行文件
add_executable(glfw_temp
        src/main.cpp
        src/glad.c
)

# 查找 OpenGL
find_package(OpenGL REQUIRED)

set(SHARED_LIBS
        libglfw.3.dylib
        OpenGL::GL
)

# 链接 GLFW 和 OpenGL 框架
target_link_libraries(glfw_temp
        ${SHARED_LIBS}
        wrapper
        app
)
add_subdirectory(wrapper)
add_subdirectory(application)

# 添加子目录
add_subdirectory(src/chapter0)
add_subdirectory(src/chapter1)
add_subdirectory(src/chapter2)
add_subdirectory(src/chapter3)
add_subdirectory(src/chapter4)
add_subdirectory(src/chapter5)
add_subdirectory(src/chapter6)
add_subdirectory(src/chapter7)