// C++ program to implement Custom Linked List and
// iterator pattern.
#include <iostream>
#include <ostream>

using namespace std;

// Custom class to handle Linked List operations
// Operations like push_back, push_front, pop_back,
// pop_front, erase, size can be added here
template <typename T>
class LinkedList
{
	// Forward declaration
	class Node;

public:
	LinkedList<T>() noexcept
	{
		// caution: static members can't be
		// initialized by initializer list
		m_spRoot = nullptr;
	}

	// Forward declaration must be done
	// in the same access scope
	class Iterator
	{
	public:
		Iterator() noexcept;
		Iterator(const Node* pNode) noexcept;
		Iterator& operator=(Node* pNode);
		Iterator& operator++();
		Iterator operator++(int);
		bool operator!=(const Iterator& iterator);
		int operator*();
	private:
		const Node* m_pCurrentNode;
	};
	static_assert(std::input_or_output_iterator<Iterator>);
private:

	class Node
	{
		T data;
		Node* pNext;

		// LinkedList class methods need
		// to access Node information
		friend class LinkedList;
	};

	// Create a new Node
	Node* GetNode(T data)
	{
		Node* pNewNode = new Node;
		pNewNode->data = data;
		pNewNode->pNext = nullptr;

		return pNewNode;
	}

	// Return by reference so that it can be used in
	// left hand side of the assignment expression
	Node*& GetRootNode()
	{
		return m_spRoot;
	}

	static Node* m_spRoot;
};

template <typename T>
/*static*/ typename LinkedList<T>::Node* LinkedList<T>::m_spRoot = nullptr;

template <typename T>
void LinkedList<T>::push_back(T data)
{
	Node* pTemp = GetNode(data);
	if (!GetRootNode())
	{
		GetRootNode() = pTemp;
	}
	else
	{
		Node* pCrawler = GetRootNode();
		while (pCrawler->pNext)
		{
			pCrawler = pCrawler->pNext;
		}

		pCrawler->pNext = pTemp;
	}
}

template <typename T>
void LinkedList<T>::Traverse()
{
	Node* pCrawler = GetRootNode();

	while (pCrawler)
	{
		cout << pCrawler->data << " ";
		pCrawler = pCrawler->pNext;
	}

	cout << endl;
}