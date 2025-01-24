cmake_minimum_required(VERSION 3.30)
project(Learn_OpenGL)

add_definitions(-DDEBUG)
set(CMAKE_CXX_STANDARD 20)
set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib)
set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/include)

# 包含头文件路径
include_directories(${GLFW_INCLUDE_DIR})

# 添加wrapper头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/wrapper)

# 添加app头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application)

# 链接库文件路径
link_directories(${GLFW_LIB_DIR})

# 添加可执行文件
add_executable(Learn_OpenGL
        src/main.cpp
        src/glad.c
)


# 在根目录的 CMakeLists.txt 中添加
set(FONTS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts")
set(FONTS_BINARY_DIR "${CMAKE_BINARY_DIR}/assets/fonts")

# 创建目标目录
file(MAKE_DIRECTORY ${FONTS_BINARY_DIR})

# 复制字体文件
file(COPY ${FONTS_SOURCE_DIR}/ DESTINATION ${FONTS_BINARY_DIR})


set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_BINARY_DIR "${CMAKE_BINARY_DIR}/assets")
set(SHADERS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADERS_BINARY_DIR "${CMAKE_BINARY_DIR}/shaders")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR}
        ${ASSETS_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SHADERS_SOURCE_DIR}
        ${SHADERS_BINARY_DIR}
    COMMENT "Copying resources to binary directory"
)

add_dependencies(Learn_OpenGL copy_resources)


# 查找 OpenGL
find_package(OpenGL REQUIRED)

if(APPLE)
        set(SHARED_LIBS
                libglfw.3.dylib
                OpenGL::GL
        )
elseif(WIN32)
        set(SHARED_LIBS
                glfw3.lib
                OpenGL::GL
        )

        add_custom_command(TARGET Learn_OpenGL POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${GLFW_LIB_DIR}/glfw3.dll"
                $<TARGET_FILE_DIR:Learn_OpenGL>
        )
endif()

# 链接 GLFW 和 OpenGL 框架
target_link_libraries(Learn_OpenGL
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
add_subdirectory(src/chapter8)
add_subdirectory(src/chapter9)
add_subdirectory(src/chapter10)