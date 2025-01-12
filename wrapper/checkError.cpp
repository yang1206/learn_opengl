#include "checkError.h"
#include <assert.h>
#include <glad/glad.h>
#include <iostream>

void checkOpenGLError() {
    GLenum errorCode = glGetError();
    std::string error_str = "";
    if (errorCode != GL_NO_ERROR) {
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error_str = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error_str = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error_str = "GL_INVALID_OPERATION";
                break;
            default:
                error_str = "Unknown error";
                break;
        }
        std::cout << "OpenGL Error: " << error_str << std::endl;
    }
    assert(errorCode == GL_NO_ERROR);
}
