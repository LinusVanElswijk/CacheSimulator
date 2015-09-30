#pragma once

#include "TypesAndUtility.h"
#include "MemoryCache.h"
#include "CacheLine.h"
#include "observing/Subject.h"
#include "observing/NotifiableSubject.h"
#include "evictionPolicies/EvictionPolicy.h"
#include <algorithm>
#include <memory>

namespace cache_simulation {
	
	class CacheLineSet : public MemoryCache {
	public:
		CacheLineSet(const int setSize, 
					 const int blockSize, 
					 const eviction_policies::EvictionPolicyType policy,
					 MemoryView& upstream
		);

		virtual ~CacheLineSet() {}

		int size() const {
			return cacheLines_.size();
		}

		virtual bool contains(const Address address) const;

		observing::Subject<CacheLineEvictionEvent>& cacheLineEviction() { return cacheLineEviction_; }

	protected:
		virtual std::vector<Byte> readBlockImplementation(const Address address);
		virtual void writeBlockImplementation(const Address address, const std::vector<Byte>& data);

	private:
		std::shared_ptr<eviction_policies::EvictionPolicy> evictionPolicy_;
		std::vector<CacheLine> cacheLines_;
		observing::NotifiableSubject<CacheLineEvictionEvent> cacheLineEviction_;

		typedef std::vector<CacheLine>::iterator CacheLineIterator;
		typedef std::vector<CacheLine>::const_iterator ConstCacheLineIterator;

		CacheLineIterator selectCachelineToEvict();
		CacheLineIterator find(const Address address);
		ConstCacheLineIterator find(const Address address) const;
		CacheLineIterator findInvalid();
	};
}