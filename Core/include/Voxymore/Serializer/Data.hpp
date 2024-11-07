//
// Created by Sayama on 07/11/2024.
//

#pragma once

#include "Voxymore/Core/Buffer.hpp"
#include "Voxymore/Core/UUID.hpp"

#define VXM_IMPL_DATA(id) static inline constexpr uint64_t ID{id}; \
virtual uint64_t GetDataID() const {return ID;}

namespace Voxymore::Core::Serializer
{
	class Data
	{
	public:
		VXM_IMPL_DATA(0);
	public:
		template<typename T>
		bool Is() { return GetDataID() == T::ID; }
	};

	class Value : public Data
	{
	public:
		Value() = default;
		~Value() = default;

		Value(const Value&) = delete;
		Value& operator=(const Value&) = delete;

		Value(Value&& other) noexcept;
		Value& operator=(Value&& other) noexcept;

		template<typename T>
		explicit Value(T value);
	public:
		VXM_IMPL_DATA(1);
	public:
		void swap(Value& other);
	public:
		void Clear();
		[[nodiscard]] bool IsEmpty() const;
	public:
		void Set(int32_t value);
		void Set(uint32_t value);
		void Set(int64_t value);
		void Set(uint64_t value);
		void Set(float_t value);
		void Set(double_t value);
		void Set(std::string value);
		void Set(Buffer value);
		void Set(ScopeBuffer value);
	public:
		[[nodiscard]] std::optional<int32_t> GetInt32() const;
		[[nodiscard]] std::optional<uint32_t> GetUint32() const;
		[[nodiscard]] std::optional<int64_t> GetInt64() const;
		[[nodiscard]] std::optional<uint64_t> GetUint64() const;
		[[nodiscard]] std::optional<float_t> GetFloat() const;
		[[nodiscard]] std::optional<double_t> GetDouble() const;
		[[nodiscard]] std::optional<std::string> GetString() const;
		[[nodiscard]] std::optional<ScopeBuffer> GetBuffer() const;

		template<typename T>
		[[nodiscard]] bool Is() const;

		template<typename T>
		bool Is(T& value) const;

		template<typename T>
		[[nodiscard]] std::optional<T> As() const;
	private:
		std::variant<std::monostate, int32_t, uint32_t, int64_t, uint64_t, float_t, double_t, std::string, ScopeBuffer> m_Value;
	};

	class Array : public Data
	{
	public:
		Array() = default;
		~Array() = default;
		Array(uint64_t capacity);
	public:
		VXM_IMPL_DATA(2);
	public:

		[[nodiscard]] uint64_t Count() const;
		[[nodiscard]] uint64_t Capacity() const;

		void Reserve(uint64_t capacity);
		void Resize(uint64_t count);

		template<typename Pred>
		std::optional<uint64_t> Find(Pred predicate);

		template<typename Pred>
		bool TryFind(uint64_t& index, Pred predicate);

		void Add(Data data);
		void Remove(uint64_t index);

		[[nodiscard]] Data& Get(uint64_t index);
		[[nodiscard]] const Data& Get(uint64_t index) const;
	private:
		std::vector<Data> m_Value;
	};

	class Dictionary : public Data
	{
		using KeyType = std::variant<int32_t, uint32_t, float, UUID, std::string>;
	public:
		Dictionary() = default;
		~Dictionary() = default;
	public:
		VXM_IMPL_DATA(3);
	public:
		void Set(const KeyType& key, Data value);
		bool TryAdd(const KeyType& key, Data value);
		void Remove(const KeyType& key);
		[[nodiscard]] bool Has(const KeyType& key) const;

		[[nodiscard]] Data& Get(const KeyType& key);
		[[nodiscard]] Data* TryGet(const KeyType& key);
	private:
		std::unordered_map<KeyType, Data> m_Value;
	};

	class Field
	{
	private:
		std::string m_Name;
		std::unique_ptr<Data> m_Value;
	};

	template<typename T>
	Value::Value(T value)
	{
		Set(value);
	}

	template<typename T>
	bool Value::Is() const
	{
		return std::holds_alternative<T>(m_Value);
	}

	template<typename T>
	std::optional<T> Value::As() const
	{
		if(Is<T>())
			return std::get<T>(m_Value);

		return std::nullopt;
	}

	template<typename T>
	bool Value::Is(T& value) const
	{
		if (Is<T>())
		{
			value = std::get<T>(m_Value);
			return true;
		}
		return false;
	}

	template<typename Pred>
	std::optional<uint64_t> Array::Find(Pred predicate)
	{
		auto it = std::find_if(m_Value.begin(), m_Value.end(), predicate);
		if(it != m_Value.end())
		{
			return it - m_Value.begin();
		}

		return std::nullopt;
	}

	template<typename Pred>
	bool Array::TryFind(uint64_t& index, Pred predicate)
	{
		auto it = std::find_if(m_Value.begin(), m_Value.end(), predicate);
		if(it != m_Value.end())
		{
			index = it - m_Value.begin();
			return true;
		}

		return false;
	}
} // namespace Voxymore::Core::Serializer



