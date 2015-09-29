#include "NWayAssociativeCache.h"

namespace cache_simulation
{
	NWayAssociativeCache::NWayAssociativeCache(const int setCount, 
											   const int setSize,
											   const int blockSize,
											   const eviction_policies::EvictionPolicyType policy,
											   MemoryView& upstream
											  )
	: MemoryCache(blockSize, upstream)
	, sets_()
	{	
		if (!isPowerOfTwo(setCount)) {
			throw std::runtime_error("setCount is not a power of 2");
		}

		if (!isPowerOfTwo(setSize)) {
			throw std::runtime_error("setSize is not a power of 2");
		}

		sets_.reserve(setCount);
		for (int i = 0; i < setCount; i++) {
			sets_.emplace_back(CacheLineSet(setSize, blockSize, policy, upstream));
		}
	}

	Address NWayAssociativeCache::associatedSetBitMask() const {
		return ((blockSize() << std::ilogb(setCount())) - 1) & ~offsetBitmask();
	}
}