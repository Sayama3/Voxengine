//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace Voxymore::Core
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t);
		UUID(const UUID&) = default;

		std::string string() const;
		inline operator std::string() const { return string(); }
		inline operator uint64_t() const { return m_UUID; }
		static UUID FromString(const std::string&);
	private:
		uint64_t m_UUID;
	};
} // namespace Voxymore::Core


namespace std
{
	template<>
	struct hash<Voxymore::Core::UUID>
	{
		inline std::size_t operator()(const Voxymore::Core::UUID& uuid) const
		{
			return hash<uint64_t>()(uuid);
		}
	};
} // namespace std
