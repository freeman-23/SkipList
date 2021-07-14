#pragma once

#include "SkipList.h"
#include <random>
#include <format>

template <class T>
template <class NodePtr>
skip_list<T>::Iterator<NodePtr>::Iterator() noexcept
	: m_ptr(nullptr)
{
}

template <class T>
template <class NodePtr>
skip_list<T>::Iterator<NodePtr>::Iterator(NodePtr ptr) noexcept
	: m_ptr(ptr)
{
}

template <class T>
template <class NodePtr>
typename skip_list<T>::reference skip_list<T>::Iterator<NodePtr>::operator*() const
{
	return *m_ptr->val;
}

template <class T>
template <class NodePtr>
typename skip_list<T>::pointer skip_list<T>::Iterator<NodePtr>::operator->() const
{
	return m_ptr->val;
}

template <class T>
template <class NodePtr>
typename skip_list<T>::template Iterator<NodePtr>& skip_list<T>::Iterator<NodePtr>::operator++()
{
	m_ptr = m_ptr->next;
	return *this;
}

template <class T>
template <class NodePtr>
typename skip_list<T>::template Iterator<NodePtr> skip_list<T>::Iterator<NodePtr>::operator++(int)
{
	const Iterator tmp = *this;
	++*this;
	return tmp;
}

template <class T>
template <class NodePtr>
typename skip_list<T>::template Iterator<NodePtr>& skip_list<T>::Iterator<NodePtr>::operator--()
{
	m_ptr = m_ptr->prev;
	return *this;
}

template <class T>
template <class NodePtr>
typename skip_list<T>::template Iterator<NodePtr> skip_list<T>::Iterator<NodePtr>::operator--(int)
{
	const Iterator tmp = *this;
	--*this;
	return tmp;
}

template <class T>
template <std::ranges::input_range Rng>
void skip_list<T>::erase(Rng rng) noexcept
{
	auto begin = rng.begin();
	while (begin != rng.end())
		erase((begin++).m_ptr);
}

template <class T>
template <std::predicate<int> Pred>
void skip_list<T>::erase_if(Pred pred) noexcept
{
	auto begin = this->begin();
	while (begin != end())
		if (pred(*begin++)) erase(begin.m_ptr->prev);
}

template <class T>
void skip_list<T>::create_new_level()
{
	const auto new_level = new node(nullptr);
	new_level->below = m_list.below;
	new_level->above = &m_list;
	m_list.below->above = new_level;
	m_list.below = new_level;
	++m_height;
}

template <class T>
void skip_list<T>::erase(const node* nd) noexcept
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

template <class T>
typename skip_list<T>::node* skip_list<T>::search(const_reference target) const noexcept
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

template <class T>
typename skip_list<T>::node* skip_list<T>::at(size_type) const noexcept
{
	// TODO
	return m_list.next;
}

template <class T>
skip_list<T>::skip_list()
	: m_size(0), m_height(1)
{
}

template <class T>
skip_list<T>::skip_list(const skip_list& other)
	: m_size(0), m_height(0)
{
	auto begin = other.begin();
	while (begin != other.end())
		push_back(*begin++);
}

template <class T>
typename skip_list<T>::node* skip_list<T>::insert(pointer target)
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

template <class T>
skip_list<T>::~skip_list() noexcept
{
	clear();
}

template <class T>
bool skip_list<T>::empty() const noexcept
{
	return m_list.next == &m_list;
}

template <class T>
typename skip_list<T>::size_type skip_list<T>::size() const noexcept
{
	return m_size;
}

template <class T>
void skip_list<T>::erase(const_iterator it) noexcept
{
	erase(it.m_ptr);
}

template <class T>
void skip_list<T>::clear() noexcept
{
	while (!empty())
		erase(cbegin());
}

template <class T>
void skip_list<T>::push_back(const_reference i)
{
	insert(new value_type(i));
}

template <class T>
void skip_list<T>::emplace(value_type&& i)
{
	insert(new value_type(std::move(i)));
}

template <class T>
void skip_list<T>::pop(const_reference i) noexcept
{
	erase(search(i));
}

template <class T>
typename skip_list<T>::iterator skip_list<T>::end() noexcept
{
	return &m_list;
}

template <class T>
typename skip_list<T>::iterator skip_list<T>::begin() noexcept
{
	return m_list.next;
}

template <class T>
typename skip_list<T>::reverse_iterator skip_list<T>::rend() noexcept
{
	return reverse_iterator(&m_list);
}

template <class T>
typename skip_list<T>::reverse_iterator skip_list<T>::rbegin() noexcept
{
	return reverse_iterator(m_list.prev);
}

template <class T>
typename skip_list<T>::const_iterator skip_list<T>::end() const noexcept
{
	return cend();
}

template <class T>
typename skip_list<T>::const_iterator skip_list<T>::cend() const noexcept
{
	return &m_list;
}

template <class T>
typename skip_list<T>::const_iterator skip_list<T>::begin() const noexcept
{
	return cbegin();
}

template <class T>
typename skip_list<T>::const_iterator skip_list<T>::cbegin() const noexcept
{
	return m_list.next;
}

template <class T>
typename skip_list<T>::const_reverse_iterator skip_list<T>::rend() const noexcept
{
	return crend();
}

template <class T>
typename skip_list<T>::const_reverse_iterator skip_list<T>::crend() const noexcept
{
	return const_reverse_iterator(&m_list);
}

template <class T>
typename skip_list<T>::const_reverse_iterator skip_list<T>::rbegin() const noexcept
{
	return crbegin();
}

template <class T>
typename skip_list<T>::const_reverse_iterator skip_list<T>::crbegin() const noexcept
{
	return const_reverse_iterator(m_list.prev);
}

template <class T>
typename skip_list<T>::const_reference skip_list<T>::back() const noexcept
{
	return m_list.prev->get();
}

template <class T>
typename skip_list<T>::const_reference skip_list<T>::front() const noexcept
{
	return m_list.next->get();
}

template <class T>
typename skip_list<T>::iterator skip_list<T>::find(const_reference target) const noexcept
{
	return search(target);
}

template <class T>
typename skip_list<T>::node* skip_list<T>::node::push_back(node* nd)
{
	nd->prev = prev;
	nd->next = this;
	prev->next = nd;
	prev = nd;
	return nd;
}

template <class T>
typename skip_list<T>::node* skip_list<T>::node::put_top(node* nd)
{
	nd->below = this;
	nd->above = above;
	above = nd;
	return nd;
}

template <class T>
void skip_list<T>::node::unlink_h() noexcept
{
	node* n = next;
	node* p = prev;
	prev->next = n;
	next->prev = p;
	next = prev = this;
}

template <class T>
skip_list<T>::node::node(pointer ptr)
	: val(ptr), prev(this), next(this), below(this), above(this)
{
}

template <class T>
skip_list<T>::node::~node() noexcept
{
	unlink_h();
	val = nullptr;
}

template <class T>
typename skip_list<T>::const_reference skip_list<T>::node::get() const
{
	return *val;
}

template <class T>
bool skip_list<T>::node::empty() const noexcept
{
	return val == nullptr;
}

template <class T>
constexpr std::string skip_list<T>::to_string() const
{
	std::string out;
	auto bottom_left = &m_list;
	std::unordered_map<const value_type*, size_t> first_row_positions;
	bool first = true;
	auto get = [&](const value_type* it, size_t f)
	{
		if (const auto i = first_row_positions.find(it); !first && i != first_row_positions.end())
			return f > i->second ? f - i->second : i->second - f;
		return size_t{1};
	};
	size_t h = m_height;
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
		out += form + "<" + std::string(get(nullptr, form.size() - 1), '-') + ">[N]\n";
		first = false;
		bottom_left = bottom_left->above;
	}
	return out;
}