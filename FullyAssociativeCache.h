#pragma once
/*
#include "CacheInterface.h"
#include "CacheLine.h"
#include "Set.h"

#include <vector>
cacheSimulation {
class FullyAssociativeCache : public CacheInterface {
public:
	FullyAssociativeCache() = delete;

	explicit FullyAssociativeCache(const int size, MemoryInterface& upstream)
	: CacheInterface(upstream)
	, cacheLines(size) {
		assert(size == cacheLines.size());
	}

	virtual ~FullyAssociativeCache() {};

	void evict(int i) {
		if (cacheLines[i].isValid() && cacheLines[i].isDirty()) {
			upstream.write(cacheLines[i].tag(), cacheLines[i].rawData());
		}
	}

	virtual int read(int * address) {
		auto match = cacheLines.find(address);
		
		if (match != cacheLines.failure()) {
			return match->data();
		}
		else {
			int rand = std::rand() % cacheLines.size();
			evict(rand);
			cacheLines[rand].setAddress(reinterpret_cast<Address>(address));
			cacheLines[rand].setData(upstream.read(address));
			cacheLines[rand].setDirty(false);
			cacheLines[rand].setValid(true);

			return cacheLines[rand].data();
		}
	}

	virtual void write(int * address, int value) {
		auto match = cacheLines.find(address);

		if (match != cacheLines.failure()) {
			match->setData(value);
			match->setDirty(true);
		}
		else {
			int rand = std::rand() % cacheLines.size();
			evict(rand);
			cacheLines[rand].setAddress(reinterpret_cast<Address>(address));
			cacheLines[rand].setData(value);
			cacheLines[rand].setDirty(true);
			cacheLines[rand].setValid(true);
		}
	}

private:
	CacheLineSet cacheLines;
};
}
*/