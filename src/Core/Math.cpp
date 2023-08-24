//
// Created by Iannis on 30/05/2023.
//

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/Math.hpp"

namespace Voxymore::Core {
// Old way
// TODO: test which one is faster!
//	glm::mat4 Math::TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
//		VXM_PROFILE_FUNCTION();
//		glm::mat4 identity(1.0f);
//		glm::mat4 t = glm::translate(identity, position); // Translation Matrix
//		glm::mat4 r = glm::toMat4(rotation); // Rotation Matrix
//		glm::mat4 s = glm::scale(identity, scale); // Scale Matrix
//		return t * r * s; // Translation * Rotation * Scale => TRS Matrix.
//	}

	glm::mat4 Math::TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
		VXM_PROFILE_FUNCTION();
		glm::mat4 trs(1.0f);
		trs = glm::translate(trs, position); // Translation Matrix
		trs = trs * glm::toMat4(rotation); // Rotation Matrix
		trs = glm::scale(trs, scale); // Scale Matrix
		return trs; // Translation * Rotation * Scale => TRS Matrix.
	}
}