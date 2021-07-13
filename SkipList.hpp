#pragma once

#include "SkipList.h"
#include <random>
#include <format>


template <class NodePtr>
skip_list::Iterator<NodePtr>::Iterator() noexcept
	: m_ptr(nullptr)
{
}

template <class NodePtr>
skip_list::Iterator<NodePtr>::Iterator(NodePtr ptr) noexcept
	: m_ptr(ptr)
{
}

template <class NodePtr>
skip_list::value_type skip_list::Iterator<NodePtr>::operator*() const
{
	return m_ptr->get();
}

template <class NodePtr>
skip_list::pointer skip_list::Iterator<NodePtr>::operator->() const
{
	return m_ptr->val;
}

template <class NodePtr>
skip_list::Iterator<NodePtr>& skip_list::Iterator<NodePtr>::operator++()
{
	m_ptr = m_ptr->next;
	return *this;
}

template <class NodePtr>
skip_list::Iterator<NodePtr> skip_list::Iterator<NodePtr>::operator++(int)
{
	const Iterator tmp = *this;
	++*this;
	return tmp;
}

template <class NodePtr>
skip_list::Iterator<NodePtr>& skip_list::Iterator<NodePtr>::operator--()
{
	m_ptr = m_ptr->prev;
	return *this;
}

template <class NodePtr>
skip_list::Iterator<NodePtr> skip_list::Iterator<NodePtr>::operator--(int)
{
	const Iterator tmp = *this;
	--*this;
	return tmp;
}

inline void skip_list::create_new_level()
{
	const auto new_level = new node(nullptr);
	new_level->below = m_list.below;
	new_level->above = &m_list;
	m_list.below->above = new_level;
	m_list.below = new_level;
	++m_height;
}

inline void skip_list::erase(const node* nd) noexcept
{
	delete nd->val;
	while (nd != &m_list)
	{
		const auto tmp = nd->above;
		delete nd;
		nd = tmp;
	}
	--m_size;
}

inline skip_list::node* skip_list::search(const_reference target) const noexcept
{
	node* v_tmp = m_list.below;
	node* h_tmp = v_tmp;
	size_type h = m_height;
	while (h-- != 0)
	{
		while (!v_tmp->next->empty() && target >= v_tmp->next->get())
			v_tmp = v_tmp->next;
		h_tmp = v_tmp;
		v_tmp = v_tmp->below;
	}
	return h_tmp;
}

inline skip_list::node* skip_list::at(size_type) const noexcept
{
	// TODO
	return m_list.next;
}

inline skip_list::skip_list()
	: m_size(0), m_height(1)
{
}

inline skip_list::node* skip_list::insert(pointer target)
{
	node* largest_small = search(*target);
	node* smallest_large = largest_small->next;

	auto new_node = new node(target);
	smallest_large->push_back(new_node);
	new_node->below = new_node->above = &m_list;

	std::mt19937 rng(std::random_device{}());
	bool flip = std::uniform_int_distribution<>{0, 1}(rng);
	size_type new_height = 1;

	while (flip)
	{
		if (++new_height > m_height)
			create_new_level();

		while (largest_small->above == &m_list) largest_small = largest_small->prev;
		largest_small = largest_small->above;

		const auto new_node_above = new node(new_node->val);
		largest_small->next->push_back(new_node_above);
		new_node = new_node->put_top(new_node_above);

		flip = std::uniform_int_distribution<>{0, 1}(rng);
	}

	++m_size;
	return new_node;
}

inline skip_list::~skip_list() noexcept
{
	clear();
}

inline bool skip_list::empty() const noexcept
{
	return m_list.next == &m_list;
}

inline skip_list::size_type skip_list::size() const noexcept
{
	return m_size;
}

inline void skip_list::erase(const_iterator it) noexcept
{
	erase(it.m_ptr);
}

inline void skip_list::clear() noexcept
{
	while (!empty())
		erase(cbegin());
}

inline void skip_list::push_back(const_reference i)
{
	insert(new value_type(i));
}

inline void skip_list::emplace(value_type&& i)
{
	insert(new value_type(std::move(i)));
}

inline void skip_list::pop(const_reference i) noexcept
{
	erase(search(i));
}

inline skip_list::iterator skip_list::end() noexcept
{
	return &m_list;
}

inline skip_list::iterator skip_list::begin() noexcept
{
	return m_list.next;
}

inline skip_list::reverse_iterator skip_list::rend() noexcept
{
	return reverse_iterator(&m_list);
}

inline skip_list::reverse_iterator skip_list::rbegin() noexcept
{
	return reverse_iterator(m_list.prev);
}

inline skip_list::const_iterator skip_list::end() const noexcept
{
	return &m_list;
}

inline skip_list::const_iterator skip_list::cend() const noexcept
{
	return end();
}

inline skip_list::const_iterator skip_list::begin() const noexcept
{
	return m_list.next;
}

inline skip_list::const_iterator skip_list::cbegin() const noexcept
{
	return begin();
}

inline skip_list::const_reverse_iterator skip_list::crend() const noexcept
{
	return const_reverse_iterator(&m_list);
}

inline skip_list::const_reverse_iterator skip_list::crbegin() const noexcept
{
	return const_reverse_iterator(m_list.prev);
}


inline skip_list::const_reference skip_list::back() const noexcept
{
	return m_list.prev->get();
}

inline skip_list::const_reference skip_list::front() const noexcept
{
	return m_list.next->get();
}

inline skip_list::iterator skip_list::find(const_reference target) const noexcept
{
	return search(target);
}

inline skip_list::node* skip_list::node::push_back(node* nd)
{
	nd->prev = prev;
	nd->next = this;
	prev->next = nd;
	prev = nd;
	return nd;
}

inline skip_list::node* skip_list::node::put_top(node* nd)
{
	nd->below = this;
	nd->above = above;
	above = nd;
	return nd;
}

inline void skip_list::node::unlink_h() noexcept
{
	node* n = next;
	node* p = prev;
	prev->next = n;
	next->prev = p;
	next = prev = this;
}

inline skip_list::node::node(pointer ptr)
	: val(ptr), prev(this), next(this), below(this), above(this)
{
}

inline skip_list::node::~node() noexcept
{
	unlink_h();
	val = nullptr;
}

inline skip_list::const_reference skip_list::node::get() const
{
	return *val;
}

inline bool skip_list::node::empty() const noexcept
{
	return val == nullptr;
}

inline bool operator==(const skip_list& lhs, const skip_list& rhs) noexcept
{
	return std::ranges::equal(lhs, rhs);
}


inline std::ostream& operator<<(std::ostream& os, const skip_list& obj)
{
	auto bottom_left = &obj.m_list;

	std::unordered_map<const skip_list::value_type*, size_t> first_row_positions;
	bool first = true;
	auto get = [&](const skip_list::value_type* it, size_t f)
	{
		if (const auto i = first_row_positions.find(it); !first && i != first_row_positions.end())
			return f > i->second ? f - i->second : i->second - f;
		return size_t{1};
	};
	size_t h = obj.m_height;
	while (h-- != 0)
	{
		std::string form = "[H]";
		auto horizon = bottom_left->next;
		while (!horizon->empty())
		{
			if (first) first_row_positions.insert({horizon->val, form.size()});
			form += std::format("<{}>[{}]",
			                    std::string(get(horizon->val, form.size() - 1), '-'), horizon->get());
			horizon = horizon->next;
		}
		if (first)
			first_row_positions.insert({nullptr, form.size()});
		os << form << "<" << std::string(get(nullptr, form.size() - 1), '-') << ">[N]" << std::endl;
		first = false;
		bottom_left = bottom_left->above;
	}
	return os;
}
