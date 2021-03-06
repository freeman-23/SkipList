#pragma once

template <class T>
class skip_list
{
public:
	using value_type = T;
	using pointer = value_type*;
	using size_type = std::size_t;
	using reference = value_type&;
	using difference_type = std::ptrdiff_t;
	using const_pointer = const value_type*;
	using const_reference = const value_type&;
	using allocator_type = std::allocator<value_type>;

	template <class NodePtr>
	struct Iterator
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = value_type;
		using pointer = value_type*;
		using reference = value_type&;

		Iterator() noexcept;
		Iterator(NodePtr ptr) noexcept;

		reference operator*() const;
		pointer operator->() const;

		Iterator& operator++();
		Iterator operator++(int);
		Iterator& operator--();
		Iterator operator--(int);

		friend bool operator==(const Iterator& lhs, const Iterator& rhs) = default;
		friend bool operator!=(const Iterator& lhs, const Iterator& rhs) = default;

		friend skip_list;
	private:
		NodePtr m_ptr;
	};

private:
	struct node
	{
		pointer val;
		node* prev;
		node* next;
		node* below;
		node* above;

		explicit node(pointer = nullptr);
		node(const node&) = default;
		node(node&&) noexcept = default;
		node& operator=(const node&) = delete;
		node& operator=(node&&) = delete;
		~node() noexcept;

		[[nodiscard]] const_reference get() const;
		[[nodiscard]] bool empty() const noexcept;

		node* push_back(node*);
		node* put_top(node*);
		void unlink_h() noexcept;
	};

public:
	using iterator = Iterator<node*>;
	using const_iterator = Iterator<const node*>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	void create_new_level();
	void erase(const node*) noexcept;
	node* insert(pointer);
	[[nodiscard]] node* at(size_type) const noexcept;
	[[nodiscard]] node* search(const_reference) const noexcept;
	[[nodiscard]] static bool flip() noexcept;

public:
	skip_list();
	skip_list(const skip_list& other);
	skip_list(skip_list&& other) = delete;
	skip_list& operator=(const skip_list& other) = delete;
	skip_list& operator=(skip_list&& other) = delete;
	~skip_list() noexcept;

	void clear() noexcept;
	void pop(const_reference) noexcept;
	void erase(const_iterator) noexcept;
	template <std::ranges::input_range Rng>
	void erase(Rng) noexcept;
	template <std::predicate<int> Pred>
	void erase_if(Pred pred) noexcept;

	void emplace(value_type&&);
	void push_back(const_reference);
	iterator insert(const_reference);

	[[nodiscard]] bool empty() const noexcept;
	[[nodiscard]] size_type size() const noexcept;
	[[nodiscard]] size_type height() const noexcept;

	[[nodiscard]] iterator end() noexcept;
	[[nodiscard]] iterator begin() noexcept;
	[[nodiscard]] reverse_iterator rend() noexcept;
	[[nodiscard]] reverse_iterator rbegin() noexcept;
	[[nodiscard]] const_iterator end() const noexcept;
	[[nodiscard]] const_iterator cend() const noexcept;
	[[nodiscard]] const_iterator begin() const noexcept;
	[[nodiscard]] const_iterator cbegin() const noexcept;
	[[nodiscard]] const_reverse_iterator rend() const noexcept;
	[[nodiscard]] const_reverse_iterator crend() const noexcept;
	[[nodiscard]] const_reverse_iterator rbegin() const noexcept;
	[[nodiscard]] const_reverse_iterator crbegin() const noexcept;

	[[nodiscard]] const_reference back() const noexcept;
	[[nodiscard]] const_reference front() const noexcept;
	[[nodiscard]] iterator find(const_reference) const noexcept;

	[[nodiscard]] constexpr std::string to_string() const;
	
	friend std::ostream& operator<<(std::ostream& os, const skip_list& obj)
	{
		return os << obj.to_string();
	}

	friend bool operator==(const skip_list& lhs, const skip_list& rhs) noexcept
	{
		return std::ranges::equal(lhs, rhs);
	}

private:
	node m_list;
	size_type m_size;
	size_type m_height;
};

#include "SkipList.hpp"
