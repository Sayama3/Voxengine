//
// Created by Iannis on 30/05/2023.
//

#ifndef LEARNOPENGL_MATHHELPER_HPP
#define LEARNOPENGL_MATHHELPER_HPP

#include <Voxymore/Core.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Voxymore::Core {
    namespace MathHelper {
        inline std::ostream& operator << (std::ostream & os, const glm::mat4 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::mat3 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::mat2 value){
            return os << glm::to_string(value);
        }
        
        inline std::ostream& operator << (std::ostream & os, const glm::vec4 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::vec3 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::vec2 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::vec1 value){
            return os << glm::to_string(value);
        }
        
        inline std::ostream& operator << (std::ostream & os, const glm::ivec4 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::ivec3 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::ivec2 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::ivec1 value){
            return os << glm::to_string(value);
        }
        
        inline std::ostream& operator << (std::ostream & os, const glm::bvec4 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::bvec3 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::bvec2 value){
            return os << glm::to_string(value);
        }
        inline std::ostream& operator << (std::ostream & os, const glm::bvec1 value){
            return os << glm::to_string(value);
        }
        
        inline std::ostream& operator << (std::ostream & os, const glm::quat value){
            return os << glm::to_string(value);
        }
    } // MathHelper
}

#endif //LEARNOPENGL_MATHHELPER_HPP
