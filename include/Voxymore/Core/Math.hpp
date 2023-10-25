//
// Created by Iannis on 30/05/2023.
//

#ifndef LEARNOPENGL_MATHHELPER_HPP
#define LEARNOPENGL_MATHHELPER_HPP

#include "Core.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace Voxymore::Core {
    class Math {
	public:
		/**
		 * Create a Transform matrix from a position, a rotation, and a scale.
		 * @param position The position as a 3D vector.
		 * @param rotation The rotation as a quaternion.
		 * @param scale The scale as a 3D vector.
		 * @return A 4x4 matrix representation of the Transform matrix.
		 */
		static glm::mat4 TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
		static void TRS(glm::mat4& trs, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::quat& rotation, glm::vec3& scale);
    };

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
	// Math
}

#endif //LEARNOPENGL_MATHHELPER_HPP
