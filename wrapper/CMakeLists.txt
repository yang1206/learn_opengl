file(GLOB_RECURSE WRAPPER ./ *.cpp)

add_library(wrapper STATIC ${WRAPPER})

target_link_libraries(wrapper ${SHARED_LIBS})
