#pragma once

#include "MemoryView.h"
#include "RamMemory.h"
#include "NWayAssociativeCache.h"
#include <algorithm>

namespace cache_simulation {

	class HierarchicalCache : public MemoryView {
		public:
			struct CacheLevel {
				CacheLevel(int setSize, int setCount) : setSize(setSize), setCount(setCount) {}

				int setSize;
				int setCount;
			};
			
			HierarchicalCache(const int blockSize, std::vector<CacheLevel> levels)
			:MemoryView(blockSize)
			, ram_(new RamMemory(blockSize))
			, levels_()
			{
				std::shared_ptr<MemoryView> upstream(ram_);

				for (int i = levels.size() - 1; i >= 0; i--) {
					MemoryView& view = *upstream;

					auto x = new NWayAssociativeCache(
						levels[i].setCount,
						levels[i].setSize,
						blockSize,
						eviction_policies::EvictionPolicyType::RANDOM,
						*upstream
					);

					std::shared_ptr<MemoryView> levelI(x);

					levels_.emplace_back(levelI);

					upstream = levelI;
				}

				std::reverse(levels_.begin(), levels_.end());
			}

			virtual bool contains(const Address address) const { return true; }

			MemoryView& level(int level) { return *levels_[level - 1]; }
			MemoryView& ram() { return *ram_; }

		protected:
			virtual std::vector<Byte> readBlockImplementation(const Address address) {
				return level(1).readBlock(address);
			}

			virtual void writeBlockImplementation(const Address address, const std::vector<Byte>& data) {
				level(1).writeBlock(address, data);
			}
			
		private:
			std::shared_ptr<MemoryView> ram_;
			std::vector<std::shared_ptr<MemoryView>> levels_;
	};
}