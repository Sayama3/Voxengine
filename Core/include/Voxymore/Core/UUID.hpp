//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <array>

namespace Voxymore::Core
{
	class UUID
	{
	public:
		using value_base = uint64_t;
		using container = std::array<value_base, 1>;
		using iterator = container::iterator;
		using const_iterator = container::const_iterator;
	public: // For creation.
		UUID();
		constexpr UUID(uint64_t id) : m_UUID(id) {}
		UUID(const UUID&) = default;
		~UUID() = default;
		[[nodiscard]] static UUID FromString(const std::string&);
	public: // For comparaison (very useful for an ID)
		[[nodiscard]] operator bool() const;
		friend bool operator==(UUID lft, UUID rht);
		friend bool operator!=(UUID lft, UUID rht);
	public: // For Logging and Displaying
		[[nodiscard]] std::string string() const;
		[[nodiscard]] inline operator std::string() const;
	public: // For Iteration and Serialization.
		[[nodiscard]] uint64_t size() const;

		[[nodiscard]] iterator begin();
		[[nodiscard]] const_iterator begin() const;
		[[nodiscard]] const_iterator cbegin() const;

		[[nodiscard]] iterator end();
		[[nodiscard]] const_iterator end() const;
		[[nodiscard]] const_iterator cend() const;

		[[nodiscard]] value_base& operator[](uint64_t index);
		[[nodiscard]] const value_base& operator[](uint64_t index) const;

		[[nodiscard]] value_base& at(uint64_t index);
		[[nodiscard]] const value_base& at(uint64_t index) const;
	private:
		union {
			uint64_t m_UUID{0u};
			container m_InnerData;
		};
	};
} // namespace Voxymore::Core


namespace std
{
	template<>
	struct hash<Voxymore::Core::UUID>
	{
		inline std::size_t operator()(const Voxymore::Core::UUID& uuid) const
		{
			static_assert(sizeof(Voxymore::Core::UUID) == sizeof(uint64_t), "The UUID has a different size than an uint64_t.");
			return hash<uint64_t>()(reinterpret_cast<const uint64_t&>(uuid));
		}
	};
} // namespace std
