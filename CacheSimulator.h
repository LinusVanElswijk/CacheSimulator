#pragma once

#include "MemoryView.h"
#include "RamMemory.h"
#include "NWayAssociativeCache.h"
#include <algorithm>

namespace cache_simulation {

	class CacheSimulator : public MemoryView {
		public:
			struct CacheLevel {
				CacheLevel(int setSize, int setCount, int latency, eviction_policies::EvictionPolicyType policy) 
				: setSize(setSize), setCount(setCount), latency(latency), policy(policy) {}

				int setSize;
				int setCount;
				int latency;
				eviction_policies::EvictionPolicyType policy;
			};
			
			CacheSimulator(const int blockSize, const std::vector<CacheLevel>& levels, const int ramLatency);

			virtual bool contains(const Address address) const { return true; }

			NWayAssociativeCache& level(int level) { return *levels_[level - 1]; }
			MemoryView& ram() { return *ram_; }
			int ramLatency() { return ramLatency_; }
			int levelLatency(int level) { return levelLatencies_[level - 1]; }
			int levelCount() const { return levels_.size(); }

		protected:
			virtual std::vector<Byte> readBlockImplementation(const Address address) {
				return level(1).readBlock(address);
			}
			virtual void writeBlockImplementation(const Address address, const std::vector<Byte>& data) {
				level(1).writeBlock(address, data);
			}

		private:
			const int ramLatency_;
			std::vector<int> levelLatencies_;
			std::shared_ptr<MemoryView> ram_;
			std::vector<std::shared_ptr<NWayAssociativeCache>> levels_;
	};
}