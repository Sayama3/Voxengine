//
// Created by Sayama on 07/11/2024.
//

#include "Data.hpp"

namespace Voxymore::Core::Serializer
{
	// ==================== VALUE ==================== //
	Value::Value(Value &&other) noexcept
	{
		swap(other);
	}

	Value &Value::operator=(Value &&other) noexcept
	{
		swap(other);
		return *this;
	}

	void Value::swap(Value& other) { std::swap(m_Value, other.m_Value); }
	void Value::Clear() { m_Value = std::monostate{}; }
	bool Value::IsEmpty() const { return std::holds_alternative<std::monostate>(m_Value); }

	void Value::Set(int32_t value) {m_Value = value;}
	void Value::Set(uint32_t value) {m_Value = value;}
	void Value::Set(int64_t value) {m_Value = value;}
	void Value::Set(uint64_t value) {m_Value = value;}
	void Value::Set(float_t value) {m_Value = value;}
	void Value::Set(double_t value) {m_Value = value;}
	void Value::Set(std::string value) {m_Value = std::move(value);}
	void Value::Set(Buffer value) {m_Value = Buffer::Copy(value);}
	void Value::Set(ScopeBuffer value) {m_Value = std::move(value);}

	std::optional<int32_t> Value::GetInt32() const { return As<int32_t>(); }
	std::optional<uint32_t> Value::GetUint32() const { return As<uint32_t>(); }
	std::optional<int64_t> Value::GetInt64() const { return As<int64_t>(); }
	std::optional<uint64_t> Value::GetUint64() const { return As<uint64_t>(); }
	std::optional<float_t> Value::GetFloat() const { return As<float_t>(); }
	std::optional<double_t> Value::GetDouble() const { return As<double_t>(); }
	std::optional<std::string> Value::GetString() const { return As<std::string>(); }
	std::optional<ScopeBuffer> Value::GetBuffer() const
	{
		if(std::holds_alternative<ScopeBuffer>(m_Value))
		{
			const ScopeBuffer& buffer = std::get<ScopeBuffer>(m_Value);
			return buffer.Duplicate();
		}
		return std::nullopt;
	}

	// ==================== ARRAY ==================== //

	Array::Array(uint64_t capacity)
	{
		Reserve(capacity);
	}

	uint64_t Array::Count() const
	{
		return m_Value.size();
	}

	uint64_t Array::Capacity() const
	{
		return m_Value.capacity();
	}

	void Array::Reserve(uint64_t capacity)
	{
		m_Value.reserve(capacity);
	}

	void Array::Resize(uint64_t count)
	{
		m_Value.resize(count);
	}

	void Array::Add(Data data)
	{
		m_Value.push_back(data);
	}

	void Array::Remove(uint64_t index)
	{
		if(index >= Count())
		{
			return;
		}

		m_Value.erase(m_Value.begin() + static_cast<int64_t>(index));
	}

	Data& Array::Get(uint64_t index)
	{
		return m_Value[index];
	}

	const Data& Array::Get(uint64_t index) const
	{
		return m_Value[index];
	}

	// ==================== DICTIONARY ==================== //

	void Dictionary::Set(const Dictionary::KeyType& key, Data value)
	{
		m_Value[key] = value;
	}

	bool Dictionary::TryAdd(const Dictionary::KeyType& key, Data value)
	{
		if(Has(key)) return false;
		Set(key,value);
		return true;
	}

	void Dictionary::Remove(const Dictionary::KeyType& key)
	{
		auto it = m_Value.find(key);
		if(it != m_Value.end())
		{
			m_Value.erase(it);
		}
	}

	bool Dictionary::Has(const Dictionary::KeyType& key) const
	{
		return m_Value.contains(key);
	}

	Data& Dictionary::Get(const Dictionary::KeyType& key)
	{
		return m_Value[key];
	}

	Data* Dictionary::TryGet(const Dictionary::KeyType& key)
	{
		if(Has(key))
		{
			return &Get(key);
		}
		return nullptr;
	}
} // namespace Voxymore::Core::Serializer

