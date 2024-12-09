//
// Created by ianpo on 11/11/2023.
//

#pragma once

namespace Voxymore::Core
{
	class UUID
	{
	public:
		UUID();
		constexpr UUID(uint64_t id) : m_UUID(id) {}
		UUID(const UUID&) = default;

		std::string string() const;
		explicit inline operator bool() const {return m_UUID != 0;}
		[[deprecated]] inline operator std::string() const { return string(); }
		[[deprecated]] inline operator uint64_t() const { return m_UUID; }
		static UUID FromString(const std::string&);
	public:
		friend bool operator ==(const UUID& lft, const UUID& rht);
		friend bool operator !=(const UUID& lft, const UUID& rht);
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
			return hash<uint64_t>()(*(uint64_t*)&uuid);
		}
	};
} // namespace std
