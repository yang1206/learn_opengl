set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/3rd/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


file(GLOB_RECURSE WRAPPER ./*.cpp ./*.h)

add_library(wrapper STATIC ${WRAPPER}  ${IMGUI_SOURCES})
target_include_directories(wrapper PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(wrapper ${SHARED_LIBS})
