//
// Created by Iannis on 30/05/2023.
//

#include "Voxymore/Core/Math.hpp"

namespace Voxymore::Core {
	glm::mat4 Math::TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
		glm::mat4 t = glm::translate(glm::mat4(1.0f), position); // Translation Matrix
		glm::mat4 r = glm::toMat4(rotation); // Rotation Matrix
        glm::mat4 s = glm::scale(glm::mat4(1.0f), scale); // Scale Matrix
		return t * r * s; // Translation * Rotation * Scale => TRS Matrix.
	}
}