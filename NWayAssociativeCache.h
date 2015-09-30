#pragma once

#include "TypesAndUtility.h"
#include "MemoryCache.h"
#include "CacheLineSet.h"
#include "evictionPolicies/EvictionPolicy.h"

#include <stdexcept>
#include <memory>

namespace cache_simulation {
	class NWayAssociativeCache : public MemoryCache {
	public:
		explicit NWayAssociativeCache(const int setCount, 
									  const int setSize, 
									  const int blockSize,
									  const eviction_policies::EvictionPolicyType policy,
									  MemoryView& upstream
		);

		virtual ~NWayAssociativeCache() {};

		int setSize() const { return sets_.front().size(); }
		int setCount() const { return static_cast<int>(sets_.size()); }

		virtual bool contains(const Address address) const { return sets_[toAssociatedSet(address)].contains(address); }

	protected:
		virtual std::vector<Byte> readBlockImplementation(const Address address) { 
			const int offsetBM = offsetBitmask();
			const int assosetBM = associatedSetBitMask();
			const int assoAddress = toAssociatedSet(address);
			return sets_[toAssociatedSet(address)].readBlock(address);
		}
		virtual void writeBlockImplementation(const Address address, const std::vector<Byte>& data) { sets_[toAssociatedSet(address)].writeBlock(address, data); }

	private:
		Address associatedSetBitMask() const;
		int toAssociatedSet(Address address) const { return (address & associatedSetBitMask()) / blockSize(); }
		
		NWayAssociativeCache() = delete;
		std::vector<CacheLineSet> sets_;
	};
}