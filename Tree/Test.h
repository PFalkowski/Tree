#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <iostream> // std::cout


//** TO DO: Aggregate test **//
template<class T = int>
struct Test
{
	template< template <class> class TestType >
	static void Run()
	{
		std::cout << "Test " << typeid(TestType<T>()).name() << " : ";
		if (TestType<T>()()) std::cout << "OK\n"; /// haha this is creepy ()()
		else std::cout << "FAIL\n";
	}
	template< template <class> class TestType, class  A >
	static void Run(const A& arg)
	{
		std::cout << "Test " << typeid(TestType<T>()).name() << " : ";
		if (TestType<T>()(arg)) std::cout << "OK\n";
		else std::cout << "FAIL\n";
	}
};


#endif /// TEST_H_INCLUDED