#pragma once
#include <algorithm>

class skip_list
{
public:
	struct node;
	using value_type = int;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	
	struct iterator
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = value_type;
		using pointer = value_type*;
		using reference = value_type&;
		
		iterator();
		iterator(node* ptr);
		
		value_type operator*() const;
		pointer operator->() const;

		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		
		friend bool operator!=(const iterator&, const iterator&);
		friend bool operator==(const iterator&, const iterator&);
		friend skip_list;
	private:
		node* m_ptr;
	};
	
	using const_iterator = const iterator&;
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
		~node();

		[[nodiscard]] const_reference get() const;
		[[nodiscard]] bool empty() const;
		
		node* push_back(node*);
		node* put_top(node*);
		void unlink_h();
	};
	
	void create_new_level();
	void erase(node*);
	node* insert(const_reference);
	[[nodiscard]] node* search(const_reference) const noexcept;

public:
	skip_list();
	~skip_list();


	void clear();
	void pop(const_reference);
	void push_back(const_reference);
	[[nodiscard]] iterator end() noexcept;
	[[nodiscard]] iterator begin() noexcept;
	[[nodiscard]] bool empty() const noexcept;
	[[nodiscard]] size_type size() const noexcept;
	[[nodiscard]] const_reference back() const noexcept;
	[[nodiscard]] const_reference front() const noexcept;
	[[nodiscard]] iterator find(const_reference) const noexcept;

	void erase(const_iterator);
	
	friend std::ostream& operator<<(std::ostream& os, const skip_list& obj);

private:
	node m_list;
	size_type m_size;
	size_type m_height;
};

#include "SkipList.hpp"
