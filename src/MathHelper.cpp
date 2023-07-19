//
// Created by Iannis on 30/05/2023.
//

#include "Voxymore/MathHelper.hpp"

namespace Voxymore::Core {
    namespace MathHelper {
        // TODO: Do better.
        glm::mat4 RotationMatrix(glm::vec3 axis, float angleInDegree) {
            auto mat = glm::mat4(1.0f);
            return glm::rotate(mat, glm::radians(angleInDegree), axis);
        }
    } // MathHelper
}