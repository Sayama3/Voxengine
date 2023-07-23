//
// Created by Iannis on 30/05/2023.
//

#include "Voxymore/Math.hpp"

namespace Voxymore::Core {
	glm::mat4 Math::TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
		glm::mat4 trs(1.0f);
		trs = glm::translate(trs, position);
		trs = trs * glm::toMat4(rotation);
		trs = glm::scale(trs, scale);
		return trs;
	}
}