#include "CacheSimulator.h"

namespace cache_simulation {

	CacheSimulator::CacheSimulator(const int blockSize, const std::vector<CacheLevel>& levels, const int ramLatency)
	: MemoryView(blockSize)
	, ram_(new RamMemory(blockSize))
	, levels_()
	, ramLatency_(ramLatency)
	, levelLatencies_()
	{
		std::shared_ptr<MemoryView> upstream(ram_);

		for (int i = levels.size() - 1; i >= 0; i--) {
			MemoryView& view = *upstream;

			auto x = new NWayAssociativeCache(
				levels[i].setCount,
				levels[i].setSize,
				blockSize,
				levels[i].policy,
				*upstream
			);
			
			std::shared_ptr<NWayAssociativeCache> levelI(x);

			levels_.emplace_back(levelI);
			levelLatencies_.emplace_back(levels[i].latency);

			upstream = levelI;
		}

		std::reverse(levels_.begin(), levels_.end());
		std::reverse(levelLatencies_.begin(), levelLatencies_.end());
	}
}