#include "CacheLineSet.h"

namespace cache_simulation {

	CacheLineSet::CacheLineSet(const int setSize, 
							   const int blockSize,
							   const eviction_policies::EvictionPolicyType policy,
							   MemoryView& upstream
							  )
	: MemoryCache(blockSize, upstream)
	, evictionPolicy_(eviction_policies::instantiate(policy))
	, cacheLines_()
	, cacheLineEviction_()
	{
		cacheLines_.reserve(setSize);
		for (int i = 0; i < setSize; i++) {
			cacheLines_.emplace_back(CacheLine(blockSize));
		}

		blockReading().attach(evictionPolicy_);
		blockWriting().attach(evictionPolicy_);
		cacheLineEviction().attach(evictionPolicy_);
	}

	bool CacheLineSet::contains(const Address address) const {
		return find(toBlockAddress(address)) != cacheLines_.end();
	}

	std::vector<Byte> CacheLineSet::readBlockImplementation(const Address address) {
		CacheLineIterator cacheLineI = find(address);

		if (cacheLineI == cacheLines_.end()) {
			cacheLineI = selectCachelineToEvict();
			cacheLineEviction_.notifyObservers(cacheLineI->blockAddress(), address);

			if (cacheLineI->isValid() && cacheLineI->isDirty()) {
				upstream_.writeBlock(cacheLineI->blockAddress(), cacheLineI->data());
			}

			cacheLineI->setBlockAddress(address);
			cacheLineI->setData(upstream_.readBlock(address));
			cacheLineI->setValid(true);
			cacheLineI->setDirty(false);
		}

		return cacheLineI->data();
	};

	void CacheLineSet::writeBlockImplementation(const Address address, const std::vector<Byte>& data) {
		CacheLineIterator cacheLineI = find(address);

		if (cacheLineI == cacheLines_.end()) {
		// miss
			cacheLineI = selectCachelineToEvict();
			cacheLineEviction_.notifyObservers(cacheLineI->blockAddress(), address);

			if ( cacheLineI->isValid() && cacheLineI->isDirty() ) {
				upstream_.writeBlock(cacheLineI->blockAddress(), cacheLineI->data());
			}

			cacheLineI->setBlockAddress(address);
			cacheLineI->setValid(true);
		}
		
		cacheLineI->setData(data);
		cacheLineI->setDirty(true);
	};

	CacheLineSet::CacheLineIterator CacheLineSet::selectCachelineToEvict() {
		CacheLineIterator cacheLineI = findInvalid();

		if (cacheLineI == cacheLines_.end()) {
			const auto toAddress = [](const CacheLine& cacheLine){ return cacheLine.blockAddress(); };
			std::vector<Address> addresses(cacheLines_.size());
			std::transform(cacheLines_.begin(), cacheLines_.end(), addresses.begin(), toAddress);
			const Address address = evictionPolicy_->select(addresses);
			cacheLineI = find(address);
		}

		return cacheLineI;
	}

	CacheLineSet::CacheLineIterator CacheLineSet::find(const Address address) {
		return std::find_if(cacheLines_.begin(), cacheLines_.end(),
			[address](const CacheLine& line) { return line.contains(address); }
		);
	}

	CacheLineSet::ConstCacheLineIterator CacheLineSet::find(const Address address) const {
		return std::find_if(cacheLines_.begin(), cacheLines_.end(),
			[address](const CacheLine& line) { return line.contains(address); }
		);
	}

	CacheLineSet::CacheLineIterator CacheLineSet::findInvalid() {
		return std::find_if(cacheLines_.begin(), cacheLines_.end(),
			[](const CacheLine& line) { return !line.isValid(); }
		);
	}
}