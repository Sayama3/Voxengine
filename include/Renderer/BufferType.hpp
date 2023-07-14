//
// Created by ianpo on 26/05/2023.
//

#ifndef LEARNOPENGL_BUFFERTYPE_HPP
#define LEARNOPENGL_BUFFERTYPE_HPP

namespace Voxymore::Core::Renderer {
    enum BufferType {
        /**
        * CubeVertex attributes
        */
        ARRAY_BUFFER = GL_ARRAY_BUFFER,
        /**
        * Atomic counter storage
        */
        ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
        /**
        * Buffer copy source
        */
        COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
        /**
        * Buffer copy destination
        */
        COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
        /**
        * Indirect compute dispatch commands
        */
        DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
        /**
        * Indirect command arguments
        */
        DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
        /**
        * CubeVertex array indices
        */
        ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
        /**
        * Pixel read target
        */
        PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
        /**
        * Texture data source
        */
        PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
        /**
        * Query result buffer
        */
        QUERY_BUFFER = GL_QUERY_BUFFER,
        /**
        * Read-write storage for shaders
        */
        SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
        /**
        * Texture data buffer
        */
        TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
        /**
        * Transform feedback buffer
        */
        TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
        /**
        * Uniform block storage
        */
        UNIFORM_BUFFER = GL_UNIFORM_BUFFER,
    };
}
#endif //LEARNOPENGL_BUFFERTYPE_HPP
