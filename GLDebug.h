#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#ifdef _DEBUG
#define GL_CHECK_ERROR() \
{ \
    GLenum err; \
    while ((err = glGetError()) != GL_NO_ERROR) { \
        std::cerr << "[OpenGL Error] " << GLErrorToString(err) << " (" << err << "): " << __FILE__ << " " << __LINE__ << std::endl; \
    } \
}
#else
#define GL_CHECK_ERROR()
#endif


inline const char* GLErrorToString(GLenum err) {
    switch (err) {
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default: return "Unknown Error";
    }
}
