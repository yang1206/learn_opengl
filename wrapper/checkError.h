#pragma once

#ifdef DEBUG
#define GL_CALL(func)                                                                                                  \
    func;                                                                                                              \
    checkOpenGLError();
#else
#define GL_CALL(func) func;
#endif

/**
 * 检查 OpenGL 错误
 */
void checkOpenGLError();
