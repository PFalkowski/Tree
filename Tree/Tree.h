// Tree implementation -*- C++ -*-

// Piotr Falkowski, 2014-04. piotr.falkowski.fm@gmail.com

// Description:
// Generic Tree data structure.
// Obeys PREORDER iteration direction. Implements basic operations as insertion & deletion.
#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED


/**************************
Do not forget to uncomment*/
//#define NDEBUG
/*************************/
#define windows defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#ifndef __cplusplus
#error "Not C++"
// #define __cplusplus 0L
#endif // __cplusplus

#if __cplusplus < 201103L && !defined(_MSC_VER)
//
#define nullptr NULL
#endif // __cplusplus

#include <iterator> // std::iterator, std::input_iterator_tag
#include <stdexcept> // std::invalid_argument
#include <cassert> // assert

#if __cplusplus >= 201103L
#include <algorithm>    // std::for_each
#endif

#ifndef NDEBUG
#include <vector>
#endif


/**
* @brief A container which offers fixed time insertion and deletion, and O(Log(N)) access. This version is not thread safe.
*
* @ingroup sequences
*
* @tparam T Type of element.
*
*/
template <class T>
class Tree
{
private:


	class Iterator;
	class Node;

	friend class Iterator;
	friend class Node;

	/**\ Data members*/

	size_t sz;
	Node *_root;

	/**\ Data members end*/

	/**< Helper struct %Node. Dtor is NOT virtual. Struct contains pointers and a value of type %T. Will never erase any pointer upon destruction. Size = 4*sizeof(pointer) + sizeof(T) */
	struct Node
	{
		/**
		* C++03: nested class do not interfere with privacy of primary class
		* C++11/14: nested class has full scope of primary class
		**/
#if __cplusplus < 201103L
		friend class Tree<T>;
#else
		Node(const Tree<T>::Node&) = default;
		Node& operator = (const Tree<T>::Node&) = default;
#endif // __cplusplus

		friend class Iterator;
		T value;

		Node* parent;
		Node* leftmost_child;
		Node* right_sibling;
		Node* left_sibling()
		{
			if (parent->leftmost_child == this)
				return nullptr;
			/// the parent has children (it should have, since this instance is a child), lets search in row
			Node* temp = parent->leftmost_child;
			/// until iteration encounters 'this', continue
			while (temp->right_sibling != this)
				temp = temp->right_sibling;/// move (pointer) one node to the right.

			return temp;
		}

		operator typename Tree<T>::Iterator()
		{
			return Iterator(this);
		}
		explicit inline Node(const T& _value = T(), typename Tree<T>::Node *_parent = nullptr,
			typename Tree<T>::Node *_leftmost_child = nullptr, typename Tree<T>::Node *_right_sibling = nullptr);
		~Node();
	};
	/**< Iterator for %Tree. Implements itrerator_traits for input iterator with Node. Size is equal to size of pointer. */
	class Iterator : public std::iterator<std::forward_iterator_tag, Node>
	{
		Node* p;
	public:
		// Iterator(T* value) : p(new Node(value, nullptr, nullptr)) {}
		/// conversion ctor
		Iterator(Node* any = nullptr) : p(any) {}
		/// Copy ctor. Makes shallow copy, so that passing iterator by value made sense
		Iterator(const Iterator& it) : p(it.p) {}
		/**< Moves pointer to next element in pre-Order element orderig and returns it, or null if the end has been reached. */

		/*Iterator& operator=(const Iterator& other)
		{
		if (this != &other)
		p = other.p;
		return *this;
		}*/
		Iterator& operator++()
		{
			Node* temp = p;
			temp = temp->leftmost_child;
			while (temp == nullptr)
			{
				if (p->right_sibling)
					temp = p->right_sibling;
				else
					p = p->parent;
				if (p == nullptr)// doszliszmy do korzenia
					break;
			}
			p = temp;
			return *this;
		}
		/**< Returns current element pointed by p. Moves pointer to next element in pre-Order element orderig. Returns null if the end is reached. */
		Iterator operator++(int)
		{
			Iterator tmp(*this);
			operator++();
			return tmp;
		}
		/**< Compares equal for iterators pointing same address. */
		bool operator==(const Iterator& it) const
		{
			return p == it.p;
		}
		/**< Compares not equal for iterators pointing different address. */
		bool operator!=(const Iterator& it) const
		{
			return p != it.p;
		}
		/**< Returns element pointed by p. */
		Node& operator*() const
		{
			return *p;// return *(operator->());
		}
		/**< Returns p (pointer to current Node). */
		Node* operator->() const
		{
			return p;
		}
		friend class Tree<T>;
		friend struct Node;
	};

	/**< Iterator for %Tree. Implements itrerator_traits for input iterator with Node. Size is equal to size of pointer. */
	class ConstIterator : public std::iterator<std::input_iterator_tag, const Node>
	{
		Node* p;
	public:
		// Iterator(T* value) : p(new Node(value, nullptr, nullptr)) {}
		/// conversion ctor
		ConstIterator(Node* any = nullptr) : p(any) {}
		/// Copy ctor. Makes shallow copy, so that passing iterator by value made sense
		ConstIterator(const Iterator& it) : p(it.p) {}

		ConstIterator(const ConstIterator& it) : p(it.p) {}
		/**< Moves pointer to next element in pre-Order element orderig and returns it, or null if the end has been reached. */

		/*Iterator& operator=(const Iterator& other)
		{
		if (this != &other)
		p = other.p;
		return *this;
		}*/
		ConstIterator& operator++()
		{
			Node* temp = p;
			temp = temp->leftmost_child;
			while (temp == nullptr)
			{
				if (p->right_sibling)
					temp = p->right_sibling;
				else
					p = p->parent;
				if (p == nullptr)// doszliszmy do korzenia
					break;
			}
			p = temp;
			return *this;
		}
		/**< Returns current element pointed by p. Moves pointer to next element in pre-Order element orderig. Returns null if the end is reached. */
		ConstIterator operator++(int)
		{
			Iterator tmp(*this);
			operator++();
			return tmp;
		}
		/**< Compares equal for iterators pointing same address. */
		bool operator==(const Iterator& it) const
		{
			return p == it.p;
		}
		/**< Compares not equal for iterators pointing different address. */
		bool operator!=(const Iterator& it) const
		{
			return p != it.p;
		}
		/**< Returns element pointed by p. */
		const Node& operator*() const
		{
			return *p;// return *(operator->());
		}
		/**< Returns p (pointer to current Node). */
		const Node* operator->() const
		{
			return p;
		}
		friend class Tree<T>;
		friend struct Node;
	};
	/** \brief Takes advantage of recursion to delete all nodes in post-order.
	* This cannot be done in destructor, without post-order moving iterator.
	* This function should be called only in destructor.
	* \param node to begin with. This should always be root.
	*
	*/
	void wipe_out(Node* node); /// Should recurrent functions be inline? Compiler can completely ignore the presence or absence of an inline qualifier, but to a certain depth it can be an optimization.
	Node* copy(const Node* const oryg, Node* parent);
public:

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef typename Tree<T>::Iterator iterator;
	typedef typename Tree<T>::ConstIterator const_iterator;
	typedef size_t size_type;

	/** Returns the size of the %Tree. This operation is atomic and O(1) in time complexity.. */
	inline const size_type& size() const;
	/**
	* @brief Inserts new element with provided %value 'under' specified parent 'after' specified 'left_sibling' position.
	* @param parent iterator pointing to node under which element shall be placed.
	* @param left_sibling iterator pointing to node 'right to' which element shall be placed.
	* @return iterator pointing to freshly created node.
	*
	* Note that this function always dynamically allocates exactly ..
	*/
	inline iterator insert(typename Tree<T>::iterator parent, typename Tree<T>::iterator left_sibling, const T & value);
	/**
	* @brief Erases element pointed by provided argument %Tree<T>::iterator.
	* @param node iterator pointing to node to be erased.
	* @return iterator pointing to node after erased.
	*
	* Note that this function only erases the element, and that if
	* the element is itself a pointer, the pointed-to memory is not touched
	* in any way. Managing the pointer is the user's responsibility.
	*/
	inline iterator erase(typename Tree<T>::iterator node);
	/**
	* Returns a read/write iterator that points to the first
	* element in the %Tree. Iteration is done in pre-order
	* element order. Operation time: constant O(1).
	*/
	inline iterator begin() const;
	/**
	* Returns a read-only (constant) iterator that points to the first
	* element in the %Tree. Iteration is done in pre-order
	* element order. Operation time: constant O(1).
	*/
	inline const_iterator cbegin() const;
	/**
	* Returns a read/write iterator that points one past the last
	* element in the %Tree. Iteration is done in pre-order
	* element order. Operation time: constant O(1).
	*/
	inline iterator end() const;
	/**
	* Returns a read-only (constant) iterator that points one past the last
	* element in the %Tree. Iteration is done in pre-order
	* element order. Operation time: constant O(1).
	*/
	inline const_iterator cend() const;
	/**
	* Same as %begin, returns a read/write iterator that points to the first
	* element in the %Tree. Iteration is done in pre-order
	* element order. Operation time: constant O(1).
	*/
	inline iterator root() const;
	/** \brief *
	* Returns a read/write iterator pointing to element associated with %Node provided as child (down in hierarchy).
	* Operation time: constant O(1).
	*
	* \param iterator containing pointer to leftmost %Node in this children row.
	* \return iterator pointing to element associated with %Node provided as child (down in hierarchy).
	* Can be NULL(nullptr in c++ >= c++11) if a node does not have any elements associated as children.
	*
	*/
	inline iterator leftmost_child(iterator node);
	/** \brief *
	* Returns a read/write iterator pointing to element associated with %Node provided as right sibling (right in hierarchy).
	* Operation time: constant O(1).
	*
	* \param iterator pointing to element associated with %Node provided as right sibling (right in hierarchy).
	* \return iterator pointing to element associated with %Node provided as child (down in hierarchy).
	* Can be NULL(nullptr in c++ >= c++11) if a node does not have any elements associated as children.
	*
	*/
	inline iterator right_sibling(iterator node);
	/**
	* @brief Creates a %Tree with root and no other elements.
	* @param root_value Value of root.
	*/
	explicit inline Tree(const T& root_value);
	/**
	* @brief Copy ctor.
	* @param other Tree to copy.
	*/
	inline Tree(const Tree<T>& other);
	/**
	* Dtor. Is not virtual. Deletes all nodes. Note, that if %T value of nodes is itself a pointer,
	* the memory is not freed.
	*/
	~Tree();

	template <class _T>
	friend std::ostream& operator<< (std::ostream& stream, const Tree<_T>& t);

	/**
	* Swap global function friendship, bcs it's not possible to swap oneself from inside of a class with std::swap.
	*/
	template <class _T>
	friend void swap(Tree<_T>& first, Tree<_T>& second);
	/**
	* @brief Copy assignment opeartor. Profits from copy-swap idiom.
	* Note the lack of ref-to-const. The copy is made by (safe) function copying arguments mechanism,
	* granting us strong exception safety for free, and no self-assignment tests overhead.
	* See http://stackoverflow.com/a/3279550/3638932 for reference.
	* @param other other Tree<T> to copy.
	*/
	inline Tree<T>& operator=(Tree<T> other);
};
template <class T>
std::ostream& operator<< (std::ostream& os, const Tree<T>& tree)
{
#if __cplusplus >= 201103L || defined(_MSC_VER) && _MSC_VER >= 1800
	bool next_flag = false;
	std::for_each(tree.begin(), tree.end(),
		[&os, &next_flag](typename Tree<T>::iterator t){ if (next_flag) os << ' '; os << t->value; next_flag = true; });
#else
	os << "This is just a tree with " << tree.size() << " leaves.\n";
#endif
	return os;
}

template <class _T>
void swap(Tree<_T>& first, Tree<_T>& second)
{
	// by swapping the members of two classes,
	// the two classes are effectively swapped
	std::swap(first.sz, second.sz);
	std::swap(first._root, second._root);
}

template <class T>
Tree<T>& Tree<T>::operator=(Tree<T> other)
{
	swap(*this, other);
	return *this;
}
template <class T>
const typename Tree<T>::size_type& Tree<T>::size() const
{
	return sz;
}
/* delete nodes in post-order. */
template <class T>
void Tree<T>::wipe_out(Node* any)
{
	if (any != nullptr)
	{
		wipe_out(any->leftmost_child);
		wipe_out(any->right_sibling);
#if !defined(NDEBUG) && defined(FULL_TEST_INFO)
		std::cerr << "wiping out node with value: " << any->value << '\n';
#endif
		delete any;
	}
}

template <class T>
Tree<T>::Tree(const T &root_value) : sz(1), _root(new Node(root_value, nullptr, nullptr, nullptr))
{

}
template <class T>
typename Tree<T>::Node* Tree<T>::copy(const Node* const oryg, Node* parent)
{
//#ifndef NDEBUG
//	if (oryg)
//		std::cerr << "Copying..\n";
//	else
//		std::cerr << "got null.";
//#endif
	if (!oryg)	return nullptr;
	Node* temp_copy = new Tree<T>::Node(oryg->value, parent); /// the new node. Children and right sibling are null.

	++sz;
	if (!parent->leftmost_child)
	{
		parent->leftmost_child = temp_copy;
	}
	if (oryg->leftmost_child)
	{
		temp_copy->leftmost_child = copy(oryg->leftmost_child, temp_copy);
	}
	if (oryg->right_sibling)
	{
		temp_copy->right_sibling = copy(oryg->right_sibling, parent);
	}
	return temp_copy;
}
template <class T>
Tree<T>::Tree(const Tree<T>& other) : sz(1), _root(new Node(other._root->value, nullptr, nullptr, nullptr))
{
	copy(other._root->leftmost_child, this->_root);
}
template <class T>
Tree<T>::~Tree()
{
	wipe_out(_root);
}
template <class T>
Tree<T>::Node::Node(const T &_value, typename Tree<T>::Node * _parent, typename Tree<T>::Node *_leftmost_child, typename Tree<T>::Node *_right_sibling)
: value(_value), parent(_parent), leftmost_child(_leftmost_child), right_sibling(_right_sibling)
{

}

template <class T>
Tree<T>::Node::~Node()
{

}
template <class T>
typename Tree<T>::iterator Tree<T>::begin() const
{
	return _root;// without conversion: *new Tree<T>::Iterator(&_root) ;
}
template <class T>
typename Tree<T>::iterator Tree<T>::root() const
{
	return _root;// without conversion: *new Tree<T>::Iterator(&_root) ;
}
template <class T>
typename Tree<T>::iterator Tree<T>::leftmost_child(iterator node)
{
	return node->leftmost_child;// or node.p->leftmost_child
}
template <class T>
typename Tree<T>::iterator Tree<T>::right_sibling(iterator node)
{
	return node->right_sibling;// or node.p->right_sibling
}
template <class T>
typename Tree<T>::const_iterator Tree<T>::cbegin() const
{
	return _root;
}

template <class T>
typename Tree<T>::iterator Tree<T>::end() const
{
	return nullptr;
}
template <class T>
typename Tree<T>::const_iterator Tree<T>::cend() const
{
	return nullptr;
}
template <class T>
typename Tree<T>::iterator Tree<T>::erase(typename Tree<T>::iterator node)
{
	if (&*node == nullptr)
		throw std::out_of_range("Cannot erase end.");
	else if (node->leftmost_child) // => (node->leftmost_child != NULL)
		throw std::invalid_argument("Cannot erase node with children, erase children first.");
	else if (&*node == _root)
		throw std::invalid_argument("Cannot erase root.");
	else
	{
		--sz; /// the size diminishes
		typename Tree<T>::iterator temp = node; //shallow copy
		++temp; /// cast to iterator and do nothing until return
		if (node->left_sibling())
			node->left_sibling()->right_sibling = node->right_sibling;/// repin left sibling pointer to right
		/// No need to repin children
		else /// we are leftmost child
			node->parent->leftmost_child = node->right_sibling; /// than we must repin our parent's leftmost_child
		delete &*node;
		return temp;
	}
}
template <class T>
typename Tree<T>::iterator Tree<T>::insert(typename Tree<T>::iterator parent, typename Tree<T>::iterator left_sibling, const T & value)
{
	++sz;
	Node * new_node = new Node(value, &*parent, nullptr, nullptr);


	if (left_sibling == nullptr)/// if the new node has to be leftmost child
	{
		if (parent.p->leftmost_child) //// and if parent currently has a child
		{
			new_node->right_sibling = parent->leftmost_child; /// repin the children effectively pushing it right
		}
		parent.p->leftmost_child = new_node;///  and insert new node as the leftmost one in the row
	}
	else /// new node is inserted between existing ones
	{
		if (left_sibling->right_sibling)/// if something to the right...
		{
			new_node->right_sibling = left_sibling.p->right_sibling; /// make space...
		}
		left_sibling.p->right_sibling = new_node; /// pin new node to left node
	}
	return *new_node;
}

/**\End\**/
#endif // TREE_H_INCLUDED
