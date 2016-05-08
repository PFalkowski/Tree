#ifndef TREETESTCASES_H_INCLUDED
#define TREETESTCASES_H_INCLUDED

#include "Tree.h"
#include <typeinfo>       // operator typeid
#include <vector> // std::vector
#include <iostream>

template <class T = int>
struct InsertLinkedList
	{
        bool operator() (const unsigned &how_many = 100)
        {
            Tree<T> tr(-1);
            std::vector<T> temp_comp(how_many + 1); // to compensate root
            temp_comp.front() = -1;
    #ifdef FULL_TEST_INFO
            std::cout << "Insertion of " << how_many << " elements under each other:\n";
    #endif
            typename Tree<T>::iterator parent = tr.begin();
            for (unsigned i = 0; i<how_many; ++i)
            {
    #ifdef FULL_TEST_INFO
                std::cout << "inserting " << i << '\n';
    #endif
                parent = tr.insert(parent, nullptr, (T)i);
                temp_comp[i + 1] = i;// to compensate root
            }
            typename std::vector<T>::iterator v_it = temp_comp.begin();
            typename Tree<T>::iterator t_it = tr.begin();


            assert(std::distance(temp_comp.begin(), temp_comp.end()) == std::distance(tr.begin(), tr.end()));
            assert(tr.size() == temp_comp.size());

            if (std::distance(temp_comp.begin(), temp_comp.end())!= std::distance(tr.begin(), tr.end())
            || tr.size() != temp_comp.size()
            || tr.size() !=std::distance(temp_comp.begin(), temp_comp.end()))
             return false;

            for (; v_it != temp_comp.end(); ++v_it, ++t_it)
            {
    #ifdef FULL_TEST_INFO
                std::cerr << "Comparing tree value (" << t_it->value << ") to expected " << (*v_it) << '\n';
    #endif

                assert((*v_it) == t_it->value);
                if ((*v_it) != t_it->value)
                    return false;
            }
    #ifdef FULL_TEST_INFO
            std::cout << "Insertion test ended succesfully.\n";
    #endif
            return true;
        }
	};
template <class T = int>
struct Assignment
	{
        bool operator() (const unsigned &how_many = 100)
        {
    #ifdef FULL_TEST_INFO
            cout << "Assignment:\n";
    #endif
            Tree<T> tr(-1);
            Tree<T> t2(-77);


            typename Tree<T>::iterator parent1 = tr.begin();
            typename Tree<T>::iterator parent2 = t2.begin();
            for (unsigned i = 0; i<how_many; ++i)
            {
    #ifdef FULL_TEST_INFO
                std::cout << "inserting " << i << '\n';
    #endif
                parent1 = tr.insert(parent1, nullptr, (T)i);
                parent2 = t2.insert(parent2, nullptr, how_many-(T)i);

            }

            t2 = tr; /// tested call

            typename Tree<T>::iterator t_it = tr.begin();
            typename Tree<T>::iterator v_it = t2.begin();

            for (; v_it != t2.end(); ++v_it, ++t_it)
            {
    #ifdef FULL_TEST_INFO
				std::cerr << "Comparing tree value (" << t_it->value << ") to expected " << v_it->value << '\n';
    #endif

                assert(v_it->value == t_it->value);
                if (v_it->value != t_it->value)
                    return false;
            }
            return true;
        }
	};
template <class T = int>
struct Copy
{
	bool operator() (const unsigned &how_many = 100)
	{
#ifdef FULL_TEST_INFO
		cout << "Copy:\n";
#endif
		Tree<T> tr(-1);


		typename Tree<T>::iterator parent1 = tr.begin();
		for (unsigned i = 0; i<how_many; ++i)
		{
#ifdef FULL_TEST_INFO
			std::cout << "inserting " << i << '\n';
#endif
			parent1 = tr.insert(parent1, nullptr, (T)i);

		}

		Tree<T> t2(tr); /// tested call

		typename Tree<T>::iterator t_it = tr.begin();
		typename Tree<T>::iterator v_it = t2.begin();

		for (; v_it != t2.end(); ++v_it, ++t_it)
		{
#ifdef FULL_TEST_INFO
			std::cerr << "Comparing tree value (" << t_it->value << ") to expected " << v_it->value << '\n';
#endif

			assert(v_it->value == t_it->value);
			if (v_it->value != t_it->value)
				return false;
		}
		return true;
	}
};
template <class T = int>
struct Swap
{
	bool operator() (const unsigned &how_many = 100)
	{
#ifdef FULL_TEST_INFO
		cout << "Assignment:\n";
#endif
		Tree<T> tr(-1);
		Tree<T> t2(-77);
		std::vector<T> tr_helper;
		std::vector<T> t2_helper;
		tr_helper.push_back(-1);
		t2_helper.push_back(-77);
		typename Tree<T>::iterator parent1 = tr.begin();
		typename Tree<T>::iterator parent2 = t2.begin();

		for (unsigned i = 0; i<how_many; ++i)
		{
#ifdef FULL_TEST_INFO
			std::cout << "inserting " << i << '\n';
#endif
			parent1 = tr.insert(parent1, nullptr, (T)i);
			parent2 = t2.insert(parent2, nullptr, how_many - (T)i);
			tr_helper.push_back( (T)i);
			t2_helper.push_back(how_many - (T)i);
		}

		swap(tr,t2); /// tested call

		typename Tree<T>::iterator tr_it = tr.begin();
		typename Tree<T>::iterator t2_it = t2.begin();
		typename std::vector<T>::iterator v1_it = tr_helper.begin();
		typename std::vector<T>::iterator v2_it = t2_helper.begin();

		for (; t2_it != t2.end(); ++tr_it, ++t2_it, ++v1_it, ++v2_it)
		{
#ifdef FULL_TEST_INFO
			std::cerr << "Comparing tree value (" << tr_it->value << ") to expected " << *v2_it << '\n';
			std::cerr << "Comparing tree value (" << t2_it->value << ") to expected " << *v1_it << '\n';
#endif

			assert(tr_it->value == *v2_it);
			assert(t2_it->value == *v1_it);
			if (tr_it->value != *v2_it || t2_it->value != *v1_it)
				return false;
		}
		return true;
	}
};
template <class T = int>
struct EraseRoot
{
	bool operator()()
	{
		Tree<T> tr(-1);
		try
		{
			tr.erase(tr.root());
		}
		catch (const std::invalid_argument& ia)
		{
			return true;
		}
		return false;
	}
};
template <class T = int>
struct LeftmostChild
{
	bool operator()(const unsigned &how_many = 100)
	{
		Tree<T> tr(0);
		assert(tr.leftmost_child(tr.begin()) == nullptr);
		if (tr.leftmost_child(tr.begin()) != nullptr)
			return false;
		typename Tree<T>::iterator it = tr.insert(tr.begin(), nullptr, 0);

		assert(it->leftmost_child == nullptr);
		if (it->leftmost_child != nullptr)
			return false;

		assert(tr.leftmost_child(it) == nullptr);
		if (tr.leftmost_child(it) != nullptr)
			return false;

		assert(tr.leftmost_child(tr.begin()) == it);
		if (tr.leftmost_child(tr.begin()) != it)
			return false;
	}
};
template <class T = int>
struct RightSibling
{
	bool operator()(const unsigned &how_many = 100)
	{
		Tree<T> tr(0);
		assert(tr.right_sibling(tr.begin()) == nullptr);
		if (tr.right_sibling(tr.begin()) != nullptr)
			return false;
		typename Tree<T>::iterator it = tr.insert(tr.begin(), nullptr, 0);

		assert(it->right_sibling == nullptr);
		if (it->right_sibling != nullptr)
			return false;

		assert(tr.right_sibling(it) == nullptr);
		if (tr.right_sibling(it) != nullptr)
			return false;

		assert(tr.right_sibling(tr.begin()) == nullptr);
		if (tr.right_sibling(tr.begin()) != nullptr)
			return false;
	}
};
template <class T = int>
struct ConstIterator
{
	bool operator()(const unsigned &how_many = 100)
	{
		Tree<T> tr(0);
		typename Tree<T>::const_iterator it = tr.insert(tr.begin(), nullptr, 1);
		it = tr.begin();
		it = tr.cbegin();
		++it;

		assert(it->value == 1);
		if (it->value != 1)
			return false;
		return true;
	}
};
	template <class T = int>
	struct InsertLinkedList2
	{
		bool operator () (const unsigned &how_many = 100)
		{
			Tree<T> tr(-1);
			std::vector<T> temp_comp(how_many + 1); // to compensate root
			temp_comp.front() = -1;
#ifdef FULL_TEST_INFO
			std::cout << "Insertion of " << how_many << " elements under root:\n";
#endif
			typename Tree<T>::iterator last = nullptr;
			for (unsigned i = 0; i<how_many; ++i)
			{
				//				std::cout << "inserting " << i << '\n';
				last = tr.insert(tr.begin(), last, (T)i);
				temp_comp[i + 1] = i;// to compensate root
			}
			typename std::vector<T>::iterator v_it = temp_comp.begin();
			typename Tree<T>::iterator t_it = tr.begin();
			assert(std::distance(temp_comp.begin(), temp_comp.end()) == std::distance(tr.begin(), tr.end()));
			assert(tr.size() == temp_comp.size());

			if (std::distance(temp_comp.begin(), temp_comp.end()) != std::distance(tr.begin(), tr.end())
				|| tr.size() != temp_comp.size()
				|| tr.size() != std::distance(temp_comp.begin(), temp_comp.end()))
				return false;

			for (; v_it != temp_comp.end(); ++v_it, ++t_it)
			{
#ifdef FULL_TEST_INFO
				std::cerr << "Comparing tree value (" << t_it->value << ") to expected " << (*v_it) << '\n';/// possible floting point number equality comparision

#endif
				assert((*v_it) == t_it->value);

				if ((*v_it) != t_it->value)
					return false;
			}
#ifdef FULL_TEST_INFO
			std::cout << "Insertion  test(2) ended succesfully.:\n";
#endif
			return true;
		}
	};
	template <class T = int>
	struct Root
	{
		bool operator () (const unsigned &how_many = 100)
		{
			Tree<T> tr(-1);
			if (tr.begin() == tr.root()) return true;
			else return false;
		}
	};
	template <class T = int>
	struct Erase
	{
		bool operator () (const unsigned &how_many = 100)
		{
#ifdef FULL_TEST_INFO
			cout << "Erase test:\n";
#endif
			Tree<T> tr(-1);


			typename Tree<T>::iterator parent1 = tr.begin();
			for (unsigned i = 0; i<how_many; ++i)
			{
#ifdef FULL_TEST_INFO
				std::cout << "inserting " << i << '\n';
#endif
				parent1 = tr.insert(parent1, nullptr, (T)i);
			}
			try
			{
				for (unsigned i = how_many; i != 0; i--)
				{
					typename Tree<T>::iterator t_it = tr.begin();
					typename Tree<T>::iterator temp_it;
					for (; t_it != tr.end(); t_it++)
					{
						temp_it = t_it;
					}
#ifdef FULL_TEST_INFO
					std::cout << "erasing Node with value " << temp_it->value << '\n';
#endif
					tr.erase(temp_it);
				}
			}
			catch (const std::invalid_argument& ia)
			{
#ifdef FULL_TEST_INFO
				std::cout << "Failing on " << ia.what() << '\n';
#endif
				return false;
			}
			catch (const std::exception& ex)
			{
#ifdef FULL_TEST_INFO
				std::cout << "Failing on " << ex.what() << '\n';
#endif
				return false;
			}
			catch (...)
			{
#ifdef FULL_TEST_INFO
				std::cout << "Failing on unknown error\n";
#endif
				return false;
			}
			assert(tr.size() == 1);
			if (tr.size() != 1)
			{
#ifdef FULL_TEST_INFO
				std::cout << "tree.size() is wrong.\n";
#endif
				return false;
			}
			return true;
		}
	};
struct TreeVisualTests
{
	template <class iterator>
	static void preOrderRecur(iterator any)
	{
		if (any != nullptr)
		{
			std::cout << any->value << ' ';
			preOrderRecur(any->leftmost_child);
			preOrderRecur(any->right_sibling);
		}
	}
	template <class iterator>
	static void InOrderRecur(iterator any)
	{
		if (any != nullptr)
		{
			InOrderRecur(any->leftmost_child);
			std::cout << any->value << ' ';
			InOrderRecur(any->right_sibling);
		}
	}
	template <class iterator>
	static void PostOrderRecur(iterator any)
	{
		if (any != nullptr)
		{
			PostOrderRecur(any->leftmost_child);
			PostOrderRecur(any->right_sibling);
			std::cout << any->value << ' ';
		}
	}
	template <class T>
	static void preOrder(Tree<T> &any)
	{
		for (typename Tree<T>::iterator it = any.begin(); it != any.end(); ++it)
			std::cout << it->value << ' ';
		std::endl(std::cout);
	}

	template<class T>
	static void TreeMemoryInformation(Tree<T> &t)
	{
		std::cout << "Size of Tree<T>: " << sizeof(Tree<T>) << " bytes"
			<< "\nSize of T: " << sizeof(T) << " bytes"
			<< "\nSize of Tree<T> instance: " << sizeof(t) << " bytes"
			<< "\nSize of Tree<T>::iterator: " << sizeof(typename Tree<T>::iterator) << " bytes"
			<< "\nSize of Tree<T>::Node: " << sizeof(typename Tree<T>::Node) << " bytes";
	}


};

#endif // TREETESTCASES_H_INCLUDED
