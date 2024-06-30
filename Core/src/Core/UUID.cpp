//
// Created by ianpo on 11/11/2023.
//


#include "Voxymore/Core/UUID.hpp"
#include <random>

namespace Voxymore::Core
{
	// Necessary element ro create a random 64bits number;
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandomEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID() : m_UUID(s_UniformDistribution(s_RandomEngine))
	{
	}

	UUID UUID::FromString(const std::string& str)
	{
		std::hash<std::string> hash_fn;
		auto hash = hash_fn(str);
		return UUID{*(uint64_t*)&hash};
	}

	std::string UUID::string() const
	{
		return std::to_string(m_UUID);
	}
	UUID::operator std::string() const { return string(); }

	UUID::iterator UUID::begin() {return m_InnerData.begin();}
	UUID::const_iterator UUID::begin() const {return m_InnerData.cbegin();}
	UUID::const_iterator UUID::cbegin() const {return m_InnerData.cbegin();}
	UUID::iterator UUID::end() {return m_InnerData.end();}
	UUID::const_iterator UUID::end() const {return m_InnerData.cend();}
	UUID::const_iterator UUID::cend() const {return m_InnerData.cend();}

	UUID::operator bool() const
	{
		return !m_UUID;
	}
	bool operator==(UUID lft, UUID rht)
	{
		return std::equal(lft.begin(), lft.end(), rht.begin());
	}
	bool operator!=(UUID lft, UUID rht)
	{
		return !(lft == rht);
	}
	uint64_t UUID::size() const
	{
		return m_InnerData.size();
	}
	UUID::value_base &UUID::operator[](uint64_t index)
	{
		return m_InnerData[index];
	}
	const UUID::value_base &UUID::operator[](uint64_t index) const
	{
		return m_InnerData[index];
	}
	UUID::value_base &UUID::at(uint64_t index)
	{
		return m_InnerData.at(index);
	}
	const UUID::value_base &UUID::at(uint64_t index) const
	{
		return m_InnerData.at(index);
	}
}// namespace Voxymore::Core