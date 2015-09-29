#include "CacheLineSet.h"

namespace cache_simulation {

	bool CacheLineSet::contains(const Address address) const {
		auto it = std::find_if(cacheLines_.begin(), cacheLines_.end(),
			[address](const CacheLine& line) { return line.contains(address); }
		);

		return it != cacheLines_.end();
	}

}