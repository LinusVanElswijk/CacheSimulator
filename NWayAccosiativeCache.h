#pragma once
/* OBSOLETE
#include "TypesAndUtility.h"
#include "CacheInterface.h"
#include "CacheLineSet.h"

#include <stdexcept>


namespace cache_simulation {
	class NWayAssociativeCache : public CacheInterface {
	public:
		NWayAssociativeCache() = delete;

		explicit NWayAssociativeCache(const int setCount, const int setSize, MemoryInterface& upstream)
		:CacheInterface(upstream)
		{
			if (!isPowerOfTwo(setCount)) { throw std::runtime_error("setCount must be a power of two."); }
			if (!isPowerOfTwo(setSize)) { throw std::runtime_error("setSize must be a power of two."); }

			sets.reserve(setCount);
			for (int i = 0; i < setCount; i++) {
				sets.emplace_back(CacheLineSet(setSize));
			}
		}

		virtual std::vector<Byte> readBytes(const Address address, const int nrOfBytes) {
			const Address setIndex = toSetIndex(address);

			auto cacheLinePtr = sets[setIndex].find(address);

			if (cacheLinePtr != sets[setIndex].failure()) {
			//cache hit
				return cacheLinePtr->readBytes(address, nrOfBytes);
			}
			else {
				//cache miss
				std::vector<Byte> bytes = upstream.readBytes(address, nrOfBytes);
				cacheLinePtr->writeBytes(address, bytes);
			}
		}


		virtual void writeBytes(const Address address, const std::vector<Byte> data) = 0;

		int setCount() const {
			return sets.size();
		}

		int setSize() const {
			return sets[0].size();
		}

	private:
		std::vector<CacheLineSet> sets;
		const Address setIndexMask_;
		
		Address setIndexBitmask() const {
			return (setCount() << 1) - 1; 
		}

		Address toSetIndex(const Address address) {
			return address & setIndexBitmask();
		}
	};
}
*/