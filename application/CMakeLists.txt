file(GLOB_RECURSE APP ./ *.cpp *.h)

add_library(app STATIC ${APP})

target_link_libraries(app ${SHARED_LIBS})
