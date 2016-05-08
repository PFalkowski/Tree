// Tree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//#define FULL_TEST_INFO
#include "Tree.h"
#include "TreeTestCases.h"
#include "Test.h"
#include <iostream> // std::cout, std::endl
#include <cstdlib> // system("PAUSE") on windows,

using namespace std;
int main()
{
	cout << "Hello! This is " << sizeof(int) * 8 << " bit environment and ISO C++ " << __cplusplus << endl;
#ifdef CPPUNIT_TESTS

	CppUnit::TestResult result;
	EraseRoot_test test_erase;

	test_erase.run(&result);
#else
	try
	{
		Test<int>::Run<InsertLinkedList>();
		Test<int>::Run<Assignment>();
		Test<int>::Run<EraseRoot>();
		Test<int>::Run<InsertLinkedList2>();
		Test<int>::Run<Copy>();
		Test<int>::Run<Swap>();
		Test<int>::Run<Erase>();
		Test<int>::Run<LeftmostChild>();
		Test<int>::Run<RightSibling>();
		Test<int>::Run<ConstIterator>();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
	}
	catch (...)
	{
		std::cerr << "unexpected error." << '\n';
	}
#endif

	//system("PAUSE");
	std::cin.get();
	return 0;
}



