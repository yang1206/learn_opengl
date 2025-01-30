set(GLM_DIR ${CMAKE_SOURCE_DIR}/3rd/glm)

file(GLOB_RECURSE APP ./ *.cpp *.h)

add_library(gl_Framework STATIC ${APP})

target_include_directories(gl_Framework PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLM_DIR}
  ${GLM_DIR}/detail
  ${GLM_DIR}/gtc
  ${GLM_DIR}/simd
  ${GLM_DIR}/gtx
)

target_link_libraries(gl_Framework ${SHARED_LIBS})
