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

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Debug/Profiling.hpp"

#ifdef VXM_DOUBLE

#define REAL_DECIMAL_DIG	DBL_DECIMAL_DIG		// # of decimal digits of rounding precision
#define REAL_DIG			DBL_DIG				// # of decimal digits of precision
#define REAL_EPSILON		DBL_EPSILON			// smallest such that 1.0+FLT_EPSILON != 1.0
#define REAL_HAS_SUBNORM	DBL_HAS_SUBNORM		// type does support subnormal numbers
#define REAL_GUARD			FLT_GUARD
#define REAL_MANT_DIG		DBL_MANT_DIG		// # of bits in mantissa
#define REAL_MAX			DBL_MAX				// max value
#define REAL_MAX_10_EXP		DBL_MAX_10_EXP		// max decimal exponent
#define REAL_MAX_EXP		DBL_MAX_EXP			// max binary exponent
#define REAL_MIN			DBL_MIN				// min normalized positive value
#define REAL_MIN_10_EXP		DBL_MIN_10_EXP		// min decimal exponent
#define REAL_MIN_EXP		DBL_MIN_EXP			// min binary exponent
#define REAL_NORMALIZE		FLT_NORMALIZE
#define REAL_RADIX			DBL_RADIX			// exponent radix
#define REAL_TRUE_MIN		DBL_TRUE_MIN		// min positive value

#else

#define REAL_DECIMAL_DIG	FLT_DECIMAL_DIG		// # of decimal digits of rounding precision
#define REAL_DIG			FLT_DIG				// # of decimal digits of precision
#define REAL_EPSILON		FLT_EPSILON			// smallest such that 1.0+FLT_EPSILON != 1.0
#define REAL_HAS_SUBNORM	FLT_HAS_SUBNORM		// type does support subnormal numbers
#define REAL_GUARD			FLT_GUARD
#define REAL_MANT_DIG		FLT_MANT_DIG		// # of bits in mantissa
#define REAL_MAX			FLT_MAX				// max value
#define REAL_MAX_10_EXP		FLT_MAX_10_EXP		// max decimal exponent
#define REAL_MAX_EXP		FLT_MAX_EXP			// max binary exponent
#define REAL_MIN			FLT_MIN				// min normalized positive value
#define REAL_MIN_10_EXP		FLT_MIN_10_EXP		// min decimal exponent
#define REAL_MIN_EXP		FLT_MIN_EXP			// min binary exponent
#define REAL_NORMALIZE		FLT_NORMALIZE
#define REAL_RADIX			FLT_RADIX			// exponent radix
#define REAL_TRUE_MIN		FLT_TRUE_MIN		// min positive value

#endif

namespace Voxymore::Core
{
	// Using Float for now. Adding 'typedef' to ease to transition if I ever want to go higher byte.
	//TODO: Search and replace the whole engine with this values.
#ifdef VXM_DOUBLE
	typedef double Real;
#else // VXM_FLOAT
	typedef float Real;
#endif

	typedef glm::qua<Real> Quat;
	typedef glm::vec<4, Real> Vec4;
	typedef glm::vec<3, Real> Vec3;
	typedef glm::vec<2, Real> Vec2;
	typedef glm::vec<1, Real> Vec1;

	typedef glm::mat<4, 4, Real> Mat4;
	typedef glm::mat<3, 3, Real> Mat3;
	typedef glm::mat<2, 2, Real> Mat2;

	class Math {
	public:
		/**
		 * Create a Transform matrix from a position, a rotation, and a scale.
		 * @param position The position as a 3D vector.
		 * @param rotation The rotation as a quaternion.
		 * @param scale The scale as a 3D vector.
		 * @return A 4x4 matrix representation of the Transform matrix.
		 */
		static Mat4 TRS(const Vec3& position, const glm::quat& rotation, const Vec3& scale = Vec3(1.0));
		static void TRS(Mat4& trs, const Vec3& position, const glm::quat& rotation, const Vec3& scale = Vec3(1.0));

		static bool DecomposeTransform(const Mat4& transform, Vec3& position, Vec3& rotation, Vec3& scale);
		static bool DecomposeTransform(const Mat4& transform, Vec3& position, glm::quat& rotation, Vec3& scale);

		template<typename T>
		inline static std::string to_string(T value)
		{
			VXM_PROFILE_FUNCTION();
			return glm::to_string(value);
		}

		template<typename T>
		inline static T Identity()
		{
			VXM_PROFILE_FUNCTION();
			return glm::identity<T>();
		}

		template<typename T>
		inline static Real SqrMagnitude(const T& value)
		{
			VXM_PROFILE_FUNCTION();
			return glm::length2(value);
		}

		template<typename T>
		inline static Real Magnitude(const T& value)
		{
			VXM_PROFILE_FUNCTION();
			return glm::length(value);
		}

		template<typename T>
		inline static T Normalize(const T& value)
		{
			VXM_PROFILE_FUNCTION();
			return glm::normalize(value);
		}

		template<int C, int R, typename T, glm::qualifier Q = glm::defaultp>
		inline static glm::mat<C,R,T,Q> Inverse(const glm::mat<C,R,T,Q>& mat)
		{
			return glm::inverse(mat);
		}

		inline static Mat4 ToMat4(const Quat& q) { VXM_PROFILE_FUNCTION(); return glm::toMat4(q); }
		inline static Mat3 ToMat3(const Quat& q) { VXM_PROFILE_FUNCTION(); return glm::toMat3(q); }

		inline static Mat4 Translate(const Mat4& mat, Vec3 translation) { VXM_PROFILE_FUNCTION(); return glm::translate(mat, translation); }
		inline static Mat4 Scale(const Mat4& mat, Vec3 scale) { VXM_PROFILE_FUNCTION(); return glm::scale(mat, scale); }

		inline static Real Pow(Real value, Real power) { VXM_PROFILE_FUNCTION(); return glm::pow(value, power); }

		/**
		 * Calculate the power of a value.
		 *
		 * This function calculates the power of a value to the given exponent.
		 * The function accepts a value of any type T and the exponent as an int64_t.
		 * It returns the result of the power operation.
		 *
		 * @tparam T The type of the value.
		 * @param value The value to calculate the power of.
		 * @param pow The exponent to raise the value to.
		 * @return The result of the power operation.
		 */
		template<typename T>
		inline static T Pow(const T& value, int64_t pow) {
			VXM_PROFILE_FUNCTION();
			T result = T(1);

			bool isNegative = pow < 0;
			auto power = isNegative ? -pow : pow;
			for (int i = 0; i < power; ++i) {
				result *= value;
			}

			return isNegative ? static_cast<T>(1) / result : result;
		}

		template<typename T>
		inline static T Pow2(const T& value) {
			VXM_PROFILE_FUNCTION();
			return value * value;
		}

		inline static Real Sqrt(Real value)
		{
			VXM_PROFILE_FUNCTION();
			return glm::sqrt(value);
		}

		inline static Vec3 Cross(const Vec3& a, const Vec3& b)
		{
			return glm::cross(a,b);
		}


		/**
		 * Calculates the dot product (also known as scalar product) of two vectors.
		 *
		 * Calculates the dot product of the two input vectors `vec1` and `vec2` and returns the result.
		 * This calculation is done using the `glm::dot` function.
		 *
		 * @tparam L The length of the vectors.
		 * @tparam T The type of the vector elements.
		 * @tparam Q The qualifier of the vector elements. It defaults to `glm::defaultp`.
		 * @param vec1 The first input vector.
		 * @param vec2 The second input vector.
		 * @return The dot product of the two input vectors.
		 *
		 * @see glm::dot
		 */
		template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
		static T Dot(const glm::vec<L,T,Q>& vec1, const glm::vec<L,T,Q>& vec2)
		{
			VXM_PROFILE_FUNCTION();
			return glm::dot(vec1, vec2);
		}

		inline constexpr static const Vec3 Gravity = Vec3(0,-9.81,0);
	}; // Math

	template<> inline std::string Math::to_string(float value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(double value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(long double value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(uint8_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(uint16_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(uint32_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(uint64_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(int8_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(int16_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(int32_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }
	template<> inline std::string Math::to_string(int64_t value) { VXM_PROFILE_FUNCTION(); return std::to_string(value); }

} // Voxymore::Core

inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Mat4 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Mat3 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Mat2 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec4 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec3 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec2 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Vec1 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const ::Voxymore::Core::Quat value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::ivec4 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::ivec3 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::ivec2 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::ivec1 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::bvec4 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::bvec3 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::bvec2 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
inline std::ostream& operator << (std::ostream & os, const glm::bvec1 value){
	VXM_PROFILE_FUNCTION();
	return os << glm::to_string(value);
}
