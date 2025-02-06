#pragma once

namespace Voxymore::Core {
	template<typename T, uint64_t MaxCount>
	class LoopBackBuffer
	{
	public:
		inline static constexpr uint64_t Capacity = MaxCount + 1;
		inline static constexpr uint64_t FirstIndex = 0;
		inline static constexpr uint64_t LastIndex = MaxCount;
	public:
		struct iterator {
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using element_type = T; // element_type is a reserved name that must be used in the definition
			using pointer = element_type*;
			using reference = element_type&;
		public:
			iterator() = default;
			~iterator() = default;
			iterator(const int64_t index, LoopBackBuffer& buffer) : buffer(&buffer), index(index % Capacity) {}

			iterator(const iterator&) = default;
			iterator& operator=(const iterator&) = default;
		public:
			reference operator*() const { return buffer->m_Data[index]; }
			pointer operator->() const {return &buffer->m_Data[index]; }

			iterator& operator+=(const int64_t offset) {
				index = (index + offset);
				index = index % static_cast<int64_t>(Capacity);
				if (index < 0) index = static_cast<int64_t>(Capacity) + index;
				return *this;
			}
			[[nodiscard]] iterator operator+(const int64_t offset) const { auto tmp = *this; tmp += offset; return tmp; }
			iterator& operator++() { (*this) += 1; return *this; }
			[[nodiscard]] iterator operator++(int) const { auto tmp = *this; ++(*this); return tmp; }

			iterator& operator-=(const int64_t offset) { *this += -offset; return *this; }
			[[nodiscard]] iterator operator-(const int64_t offset) const { auto tmp = *this; tmp -= offset; return tmp; }
			iterator& operator--() { (*this) -= 1; return *this; }
			[[nodiscard]] iterator operator--(int) const { auto tmp = *this; --(*this); return tmp; }

			reference operator[](const int64_t index) const {return buffer->operator[](index);}

			[[nodiscard]] bool operator==(const iterator& other) const { return buffer == other.buffer && index == other.index; }
			[[nodiscard]] bool operator!=(const iterator& other) const { return !(*this == other); }
		private:
			[[nodiscard]] LoopBackBuffer* get_buffer() {return buffer;}
			[[nodiscard]] const LoopBackBuffer* get_buffer() const {return buffer;}
			[[nodiscard]] int64_t get_index() const {return index;}
		private:
			LoopBackBuffer* buffer = nullptr;
			int64_t index = 0;
		};
		struct const_iterator {
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using element_type = T; // element_type is a reserved name that must be used in the definition
			using pointer = const element_type*;
			using reference = const element_type&;
		public:
			const_iterator() = default;
			~const_iterator() = default;
			const_iterator(const int64_t index, const LoopBackBuffer& buffer) : buffer(&buffer), index(index % Capacity) {}
			explicit const_iterator(const iterator& iterator) : buffer(iterator.get_buffer()), index(buffer->get_index() % Capacity) {}

			const_iterator(const const_iterator&) = default;
			const_iterator& operator=(const const_iterator&) = default;
		public:
			reference operator*() const { return buffer->m_Data[index]; }
			pointer operator->() const {return &buffer->m_Data[index]; }

			const_iterator& operator+=(const int64_t offset) {
				index = (index + offset);
				index = index % static_cast<int64_t>(Capacity);
				if (index < 0) index = static_cast<int64_t>(Capacity) + index;
				return *this;
			}
			[[nodiscard]] const_iterator operator+(const int64_t offset) const { auto tmp = *this; tmp += offset; return tmp; }
			const_iterator& operator++() { (*this) += 1; return *this; }
			[[nodiscard]] const_iterator operator++(int) const { auto tmp = *this; ++(*this); return tmp; }

			const_iterator& operator-=(const int64_t offset) { *this += -offset; return *this; }
			[[nodiscard]] const_iterator operator-(const int64_t offset) const { auto tmp = *this; tmp -= offset; return tmp; }
			const_iterator& operator--() { (*this) -= 1; return *this; }
			[[nodiscard]] const_iterator operator--(int) const { auto tmp = *this; --(*this); return tmp; }

			reference operator[](const int64_t index) const {return buffer->operator[](index);}

			[[nodiscard]] bool operator==(const const_iterator& other) const { return buffer == other.buffer && index == other.index; }
			[[nodiscard]] bool operator!=(const const_iterator& other) const { return !(*this == other); }
		public:
			[[nodiscard]] const LoopBackBuffer* get_buffer() const {return buffer;}
			[[nodiscard]] int64_t get_index() const {return index;}
		private:
			const LoopBackBuffer* buffer = nullptr;
			int64_t index = 0;
		};
	public:
		LoopBackBuffer() = default;
		~LoopBackBuffer() = default;
	public:
		std::optional<T> push_front(const T& item);
		std::optional<T> push_front(T&& item);
		std::optional<T> pop_back();
		[[nodiscard]] iterator begin() { return iterator{ static_cast<int64_t>(m_Begin), *this }; }
		[[nodiscard]] iterator end() { return iterator{ static_cast<int64_t>(m_End), *this }; }
		[[nodiscard]] const_iterator cbegin() const { return const_iterator{ static_cast<int64_t>(m_Begin), *this }; }
		[[nodiscard]] const_iterator cend() const { return const_iterator{ static_cast<int64_t>(m_End), *this }; }
		[[nodiscard]] uint64_t size() const;
		[[nodiscard]] uint64_t max_size() const { return MaxCount; }
		[[nodiscard]] uint64_t capacity() const { return Capacity; }
		[[nodiscard]] bool empty() const;
	public:
		const T& operator[](const int64_t index) const {return m_Data[get_index(index)];}
		T& operator[](const int64_t index) {return m_Data[get_index(index)];}
	private:
		[[nodiscard]] static uint64_t increase_index(uint64_t index);
		[[nodiscard]] static uint64_t decrease_index(uint64_t index);

		[[nodiscard]] uint64_t get_index(uint64_t index) const;
		[[nodiscard]] uint64_t get_index(int64_t index) const;
	private:
		T m_Data[Capacity];
		uint64_t m_Begin = 0;
		uint64_t m_End = 0;
	};

	static_assert(std::bidirectional_iterator<LoopBackBuffer<float, 5>::iterator>);


	template<typename T, std::size_t Capacity>
	inline std::optional<T> LoopBackBuffer<T, Capacity>::push_front(const T& item)
	{
		return push_front(std::move(T(item)));
	}

	template<typename T, std::size_t Capacity>
	inline std::optional<T> LoopBackBuffer<T, Capacity>::push_front(T&& item)
	{
		std::optional<T> last = std::nullopt;
		const uint64_t new_index = m_Begin == 0 ? Capacity - 1 : m_Begin - 1;
		if (new_index == m_End)
		{
			last = pop_back();
		}
		m_Data[new_index] = std::move(item);
		m_Begin = new_index;
		return last;
	}

	template<typename T, std::size_t Capacity>
	inline std::optional<T> LoopBackBuffer<T, Capacity>::pop_back()
	{
		if (empty()) return std::nullopt;

		const uint64_t last_index = m_End == 0 ? Capacity - 1 : m_End - 1;
		m_End = last_index;
		return std::move(m_Data[last_index]);

	}

	template<typename T, std::size_t Capacity>
	inline uint64_t LoopBackBuffer<T, Capacity>::size() const
	{
		if (m_Begin < m_End)
		{
			return (m_End - m_Begin);
		}
		else if (m_Begin > m_End) {
			return (Capacity - m_Begin) + m_End;
		}
		else {
			return 0;
		}
	}

	template<typename T, std::size_t Capacity>
	inline bool LoopBackBuffer<T, Capacity>::empty() const
	{
		return m_Begin == m_End;
	}

	template<typename T, std::size_t MaxCount>
	uint64_t LoopBackBuffer<T, MaxCount>::increase_index(const uint64_t index) {
		return index == LastIndex ? 0 : index + 1;
	}

	template<typename T, std::size_t MaxCount>
	uint64_t LoopBackBuffer<T, MaxCount>::decrease_index(const uint64_t index) {
		return index == 0 ? LastIndex : index - 1;
	}

	template<typename T, std::size_t MaxCount>
	uint64_t LoopBackBuffer<T, MaxCount>::get_index(const uint64_t index) const {
		const uint64_t count = size();
		const uint64_t mod_index = index % count;
		const uint64_t offset_index = m_Begin + mod_index;
		return offset_index % Capacity;
	}

	template<typename T, std::size_t MaxCount>
	uint64_t LoopBackBuffer<T, MaxCount>::get_index(const int64_t index) const {
		const uint64_t count = size();
		int64_t mod_index = index % count;
		if (mod_index < 0) mod_index = count - mod_index;
		const uint64_t offset_index = m_Begin + mod_index;
		return offset_index % Capacity;
	}
}
