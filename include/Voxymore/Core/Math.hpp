//
// Created by Iannis on 30/05/2023.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/hash.hpp>
#include <iostream>
#include <cmath>
#include <cstdint>

namespace Voxymore::Core
{
	// Using Float for now. Adding 'typedef' to ease to transition if I ever want to go higher byte.
	//TODO: Search and replace the whole engine with this values.
#ifdef VXM_DOUBLE
	typedef double Real;

	typedef glm::dquat Quat;
	typedef glm::dvec4 Vec4;
	typedef glm::dvec3 Vec3;
	typedef glm::dvec2 Vec2;
	typedef glm::dvec1 Vec1;

	typedef glm::dmat4 Mat4;
	typedef glm::dmat3 Mat3;
	typedef glm::dmat2 Mat2;
#else // VXM_FLOAT
	typedef float Real;

	typedef glm::quat Quat;
	typedef glm::vec4 Vec4;
	typedef glm::vec3 Vec3;
	typedef glm::vec2 Vec2;
	typedef glm::vec1 Vec1;

	typedef glm::mat4 Mat4;
	typedef glm::mat3 Mat3;
	typedef glm::mat2 Mat2;
#endif

	class Math {
	public:
		/**
		 * Create a Transform matrix from a position, a rotation, and a scale.
		 * @param position The position as a 3D vector.
		 * @param rotation The rotation as a quaternion.
		 * @param scale The scale as a 3D vector.
		 * @return A 4x4 matrix representation of the Transform matrix.
		 */
		static Mat4 TRS(const Vec3& position, const glm::quat& rotation, const Vec3& scale);
		static void TRS(Mat4& trs, const Vec3& position, const glm::quat& rotation, const Vec3& scale);

		static bool DecomposeTransform(const Mat4& transform, Vec3& position, Vec3& rotation, Vec3& scale);
		static bool DecomposeTransform(const Mat4& transform, Vec3& position, glm::quat& rotation, Vec3& scale);

		template<typename T>
		inline static std::string to_string(T value)
		{
			return glm::to_string(value);
		}

		template<typename T>
		inline static T Identity()
		{
			return glm::identity<T>();
		}

		template<typename T>
		inline static Real Magnitude(const T& value)
		{
			return glm::length(value);
		}

		template<typename T>
		inline static void Normalize(T& value)
		{
			value = glm::normalize(value);
		}

		inline static Mat4 ToMat4(const Quat& q) { return glm::toMat4(q); }
		inline static Mat3 ToMat3(const Quat& q) { return glm::toMat3(q); }

		inline static Mat4 Translate(const Mat4& mat, Vec3 translation) { return glm::translate(mat, translation); }
		inline static Mat4 Scale(const Mat4& mat, Vec3 scale) { return glm::scale(mat, scale); }

		inline static Real Pow(Real value, Real power) { return glm::pow(value, power); }
	}; // Math

	template<> inline std::string Math::to_string(float value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(double value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(long double value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(unsigned char value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(unsigned short value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(unsigned int value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(unsigned long int value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(unsigned long long int value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(signed char value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(signed short value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(signed int value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(signed long int value) { return std::to_string(value); }
	template<> inline std::string Math::to_string(signed long long int value) { return std::to_string(value); }

} // Voxymore::Core

inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Mat4 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Mat3 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Mat2 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec4 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec3 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec2 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec1 value){
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Quat value){
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
