
#include <LPHashTable.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

void fail()
{
	cout << "TEST_FAILED\n";
	exit(1);
}

void test_LPHashTable()
{
	LPHashTable x;
	cout << "TEST_PASSED\n";
}

int main(int argc, char *argv[])
{
	test_LPHashTable();
	return 0;
}
