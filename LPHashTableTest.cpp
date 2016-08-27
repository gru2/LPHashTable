
#include <LPHashTable.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

void fail()
{
	cout << "TEST_FAILED\n";
	exit(1);
}

class TestEntry
{
public:
	TestEntry() : key(0) {}
	TestEntry(unsigned k, unsigned v, unsigned h) : key(k), value(v), hash_(h) {}
	~TestEntry() {}

	unsigned getKey() const { return key; }
	unsigned getValue() const { return value; }
	unsigned getHash() const { return hash_; }
	bool empty() const { return key == 0; }
	void clear() { key = 0; }
private:
	unsigned key;
	unsigned value;
	unsigned hash_;
public:
	static unsigned hash(unsigned x) { return x ^ (x * 4347 + 17); }
};

void test_LPHashTable()
{
	LPHashTable<TestEntry, unsigned, unsigned> ht;
	ht.insert(1, 5);
	std::cout << "ht.size() = " << ht.size() << "\n";
	ht.erase(1);
	std::cout << "ht.size() = " << ht.size() << "\n";
	cout << "TEST_PASSED\n";
}

int main(int argc, char *argv[])
{
	test_LPHashTable();
	return 0;
}
