//
// Created by Iannis on 30/05/2023.
//

#ifndef LEARNOPENGL_MATHHELPER_HPP
#define LEARNOPENGL_MATHHELPER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Voxymore::Core {
    namespace MathHelper {
        glm::mat4 RotationMatrix(glm::vec3 axis, float angleInDegree);
    } // MathHelper
}

#endif //LEARNOPENGL_MATHHELPER_HPP
