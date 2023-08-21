cmake_minimum_required(VERSION 3.24)

target_sources(${LIBRARY_TARGET_NAME} PRIVATE
        Voxymore/OpenGL/OpenGLContext.cpp
        Voxymore/OpenGL/OpenGLContext.hpp
        Voxymore/OpenGL/OpenGLBuffer.cpp
        Voxymore/OpenGL/OpenGLBuffer.hpp
        Voxymore/OpenGL/OpenGLShader.cpp
        Voxymore/OpenGL/OpenGLShader.hpp
        Voxymore/OpenGL/OpenGLVertexArray.cpp
        Voxymore/OpenGL/OpenGLVertexArray.hpp
        Voxymore/OpenGL/OpenGLRenderAPI.cpp
        Voxymore/OpenGL/OpenGLRenderAPI.hpp
        Voxymore/OpenGL/OpenGLTexture2D.cpp
        Voxymore/OpenGL/OpenGLTexture2D.hpp
        Voxymore/OpenGL/OpenGLFramebuffer.cpp
        Voxymore/OpenGL/OpenGLFramebuffer.hpp
        )

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>")