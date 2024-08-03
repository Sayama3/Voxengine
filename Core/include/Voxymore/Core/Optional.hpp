//
// Created by ianpo on 02/08/2024.
//

#pragma once

#include <utility>

namespace Voxymore::Core
{
	template<typename T, typename TNullArg, TNullArg NullValue>
	class Optional
	{
	public:
		constexpr Optional() = default;
		~Optional() = default;
		template<typename ... Args>
		Optional(Args&&... args) : m_Value(std::forward<Args>(args)...) {}
	public:
		[[nodiscard]] bool has_value() const {return m_Value != NullValue;}
		T& value() {return m_Value;}
		const T& value() const {return m_Value;}

		void reset() {m_Value = NullValue;}
	public:
		explicit operator bool() const {return has_value();}
		explicit operator T&() {return m_Value();}
		explicit operator const T&() const {return m_Value();}
	private:
		T m_Value{NullValue};
	};

	template<typename T, T NullValue>
	using SimpleOptional = Optional<T, T, NullValue>;

	template<typename T>
	using OptionalPtr = SimpleOptional<T*, nullptr>;
}
// Voxymore::Core