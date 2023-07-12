//
// Created by ianpo on 27/05/2023.
//

#ifndef LEARNOPENGL_OPENGLTYPE_HPP
#define LEARNOPENGL_OPENGLTYPE_HPP

#include "glad/glad.h"

namespace Voxymore::Core::Renderer {
    enum GLType {
        /**
         * Accepted by:\n\n
         * - glVertexAttribPointer\\nn
         * - glVertexAttribIPointer\n\n
         */
        BYTE = GL_BYTE,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribIPointer\n
         */
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribIPointer\n
         */
        SHORT = GL_SHORT,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribIPointer\n
         */
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribIPointer\n
         */
        INT = GL_INT,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribIPointer\n
         */
        UNSIGNED_INT = GL_UNSIGNED_INT,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribIPointer\n
         */
        HALF_FLOAT = GL_HALF_FLOAT,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         */
        FLOAT = GL_FLOAT,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         * - glVertexAttribLPointer\n
         */
        DOUBLE = GL_DOUBLE,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         */
        FIXED = GL_FIXED,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         */
        INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         */
        UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
        /**
         * Accepted by:\n
         * - glVertexAttribPointer\n
         */
        UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,
    };

    inline unsigned int GetSizeOfGLType(enum GLType type) {
        switch (type) {
            case BYTE:
                return sizeof(GLbyte);
            case UNSIGNED_BYTE:
                return sizeof(GLuint);
            case SHORT:
                return sizeof(GLshort);
            case UNSIGNED_SHORT:
                return sizeof(GLushort);
            case INT:
                return sizeof(GLint);
            case UNSIGNED_INT:
                return sizeof(GLuint);
            case HALF_FLOAT:
                return sizeof(GLhalf);
            case FLOAT:
                return sizeof(GLfloat);
            case DOUBLE:
                return sizeof(GLdouble);
            case FIXED:
                return sizeof(GLfixed);
            default:
                return 0;
        }
    }

    enum GLDrawMode {
        POINTS = GL_POINTS,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        LINES = GL_LINES,
        LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY = GL_LINES_ADJACENCY,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
        PATCHES = GL_PATCHES
    };
}

#endif //LEARNOPENGL_OPENGLTYPE_HPP
