//
// Created by ianpo on 27/05/2023.
//

#ifndef LEARNOPENGL_TEXTUREENUMS_HPP
#define LEARNOPENGL_TEXTUREENUMS_HPP

namespace Voxymore::Core::Renderer {
    enum TextureType {
        TEXTURE_1D = GL_TEXTURE_1D,
        TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
        TEXTURE_2D = GL_TEXTURE_2D,
        TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
        TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
        TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
        TEXTURE_3D = GL_TEXTURE_3D,
        TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
        TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
        TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE
    };

    enum TextureParameterName {
        DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE,
        TEXTURE_BASE_LEVEL = GL_TEXTURE_BASE_LEVEL,
        TEXTURE_COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC,
        TEXTURE_COMPARE_MODE = GL_TEXTURE_COMPARE_MODE,
        TEXTURE_LOD_BIAS = GL_TEXTURE_LOD_BIAS,
        TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
        TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER,
        TEXTURE_MIN_LOD = GL_TEXTURE_MIN_LOD,
        TEXTURE_MAX_LOD = GL_TEXTURE_MAX_LOD,
        TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL,
        TEXTURE_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
        TEXTURE_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
        TEXTURE_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
        TEXTURE_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,
        TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
        TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T,
        TEXTURE_WRAP_R = GL_TEXTURE_WRAP_R
    };

    enum TextureWrapping {
        /**
         * The default behavior for textures. Repeats the texture image.
         */
        REPEAT = GL_REPEAT,
        /**
         * Same as REPEAT but mirrors the image with each repeat.
         */
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        /**
         * Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
         */
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        /**
         * Coordinates outside the range are now given a user-specified border color.
         */
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    };

    enum TextureFormat {
        NONE = 0,
        RED = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        BGR = GL_BGR,
        RGBA = GL_RGBA,
        BGRA = GL_BGRA,
        RED_INTEGER = GL_RED_INTEGER,
        RG_INTEGER = GL_RG_INTEGER,
        RGB_INTEGER = GL_RGB_INTEGER,
        BGR_INTEGER = GL_BGR_INTEGER,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        BGRA_INTEGER = GL_BGRA_INTEGER,
        STENCIL_INDEX = GL_STENCIL_INDEX,
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL
    };

// TODO: Add documentation
    enum TextureSlot : unsigned int {
        TEXTURE0 = GL_TEXTURE0,
        TEXTURE1 = GL_TEXTURE1,
        TEXTURE2 = GL_TEXTURE2,
        TEXTURE3 = GL_TEXTURE3,
        TEXTURE4 = GL_TEXTURE4,
        TEXTURE5 = GL_TEXTURE5,
        TEXTURE6 = GL_TEXTURE6,
        TEXTURE7 = GL_TEXTURE7,
        TEXTURE8 = GL_TEXTURE8,
        TEXTURE9 = GL_TEXTURE9,
        TEXTURE10 = GL_TEXTURE10,
        TEXTURE11 = GL_TEXTURE11,
        TEXTURE12 = GL_TEXTURE12,
        TEXTURE13 = GL_TEXTURE13,
        TEXTURE14 = GL_TEXTURE14,
        TEXTURE15 = GL_TEXTURE15,
        TEXTURE16 = GL_TEXTURE16,
        TEXTURE17 = GL_TEXTURE17,
        TEXTURE18 = GL_TEXTURE18,
        TEXTURE19 = GL_TEXTURE19,
        TEXTURE20 = GL_TEXTURE20,
        TEXTURE21 = GL_TEXTURE21,
        TEXTURE22 = GL_TEXTURE22,
        TEXTURE23 = GL_TEXTURE23,
        TEXTURE24 = GL_TEXTURE24,
        TEXTURE25 = GL_TEXTURE25,
        TEXTURE26 = GL_TEXTURE26,
        TEXTURE27 = GL_TEXTURE27,
        TEXTURE28 = GL_TEXTURE28,
        TEXTURE29 = GL_TEXTURE29,
        TEXTURE30 = GL_TEXTURE30,
        TEXTURE31 = GL_TEXTURE31,
        COUNT = (GL_MAX_TEXTURE_IMAGE_UNITS - GL_TEXTURE0),
    };

    enum TextureUsage {
        Other,
        Ambient,
        Diffuse,
        Specular,
        Normal,
        Height,
        Emissive,
        Count,
    };

    inline std::string TextureUsageToName(TextureUsage textureUsage) {
        std::string name;
        switch (textureUsage) {
            case Other:
                name = "texture_other";
                break;
            case Ambient:
                name = "texture_ambient";
                break;
            case Diffuse:
                name = "texture_diffuse";
                break;
            case Specular:
                name = "texture_specular";
                break;
            case Normal:
                name = "texture_normal";
                break;
            case Height:
                name = "texture_height";
                break;
            case Emissive:
                name = "texture_emissive";
                break;
            default:
                name = "texture_unknown";
                break;
        }
        return name;
    }
}
#endif //LEARNOPENGL_TEXTUREENUMS_HPP
