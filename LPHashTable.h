#ifndef __LP_HASH_TABLE_H
#define __LP_HASH_TABLE_H

// E - entry type, K - key type, V - value type
template<typename E, typename K, typename V>
class LPHashTable
{
public:
	unsigned capacity() const { return capacity_; }
	unsigned size() const { return size_; }
	LPHashTable<E, K, V>() : data(0), size_(0), capacity_(0), maxOccupancyRate(0.3) { }
	void insert(const K &key, const V &value, unsigned hash);
	void insert(const K &key, const V &value);
	V &operator[](const K &key);
	E *find(const K &key) const;
	void erase(const K &key);
	E *begin() const { return data; }
	E *end() const { return data + capacity_; }
protected:
	void grow();

	E *data;
	unsigned capacity_;
	unsigned size_;
	float maxOccupancyRate;
public:
	float getMaxOccupancyRate() const { return maxOccupancyRate; }
	void setMaxOccupancyRate(float x) { maxOccupancyRate = x; }
};

template<typename E, typename K, typename V>
void LPHashTable<E, K, V>::insert(const K &key, const V &value, unsigned hash)
{
	if (float(size_ + 1) >= capacity_ * maxOccupancyRate)
		grow();
	unsigned index = hash % capacity_;
	for (;;)
	{
		if (data[index].empty())
		{
			data[index] = E(key, value, hash);
			size_++;
			return;
		}
		index++;
		if (index >= capacity_)
			index = 0;
	}
}

template<typename E, typename K, typename V>
void LPHashTable<E, K, V>::insert(const K &key, const V &value)
{
	unsigned hash = E::hash(key);
	insert(key, value, hash);
}

template<typename E, typename K, typename V>
void LPHashTable<E, K, V>::grow()
{
	unsigned oldCapacity = capacity_;
	E *oldData = data;
	capacity_ = capacity_ << 1;
	if (capacity_ == 0)
		capacity_ = 8;
	data = new E[capacity_];
	E *p = oldData;
	for (unsigned i = 0; i < oldCapacity; i++)
	{
		if (!p->empty())
			insert(p->getKey(), p->getValue(), p->getHash());
		p++;
	}
	delete[] oldData;
}

template<typename E, typename K, typename V>
V &LPHashTable<E, K, V>::operator[](const K &key)
{
	E *p = find(key);
	return p->getValue();
}

template<typename E, typename K, typename V>
E *LPHashTable<E, K, V>::find(const K &key) const
{
	unsigned hash = E::hash(key);
	unsigned index = hash % capacity_;
	for (;;)
	{
		E *it = data + index;
		if (it->empty())
			return end();
		if (hash == it->getHash())
		{
			if (it->getKey() == key)
			{
				return it;
			}
		}
		index++;
		if (index >= capacity_)
			index = 0;
	}
}

// hash - hash value of the element
// lb - interval lower bound ( lb >=0 && lb < capacity )
// ub - interval upper bound and ( ub = lb + interval size )
// capacity - the capacity of the hash table
static bool checkIfElementHasIndexInsindeInterval(unsigned hash, unsigned lb, unsigned ub, unsigned capacity)
{
	int t = hash % capacity;
	if (t < lb)
		t += capacity;
	if (t >= lb && t < ub)
		return true;
	return false;
}

template<typename E, typename K, typename V>
void LPHashTable<E, K, V>::erase(const K &key)
{
	E *p = find(key);
	if (p == end())
		return;
	unsigned index = p - data;

	unsigned A = 0, i = index, lb;
	for (;;)
	{
		lb = i;
		if (i == 0)
			i = capacity_;
		i--;
		E *it = data + i;
		if (it->empty())
			break;
		A++;
	}
	unsigned ub = lb + A + 1, B = 0;
	for (;;)
	{
		index++;
		if (index == capacity_)
			index = 0;
		E *p2 = data + index;
		if (p2->empty())
			break;
		B++;
		unsigned hash = p2->getHash();
		if (checkIfElementHasIndexInsindeInterval(hash, lb, ub, capacity_))
		{
			*p = *p2; // Move element to fillin the hole.
			p2->clear();
			p = p2; // p points to the new hole.
			ub += B;
			B = 0;
		}
	}
	size_--;
}

#endif
