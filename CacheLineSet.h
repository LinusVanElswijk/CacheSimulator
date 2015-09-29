#pragma once

#include "MemoryCache.h"
#include "CacheLine.h"
#include "observing/Subject.h"
#include "observing/NotifiableSubject.h"
#include "evictionPolicies/EvictionPolicy.h"
#include <algorithm>
#include <memory>

namespace cache_simulation {
	struct CacheLine;

	class CacheLineSet : public MemoryCache {
	public:
		CacheLineSet(const int setSize, 
					 const int blockSize, 
					 std::unique_ptr<eviction_policies::EvictionPolicy> policy,
					 MemoryView& upstream
		);

		virtual ~CacheLineSet() {}

		int size() const {
			return cacheLines_.size();
		}

		virtual bool contains(const Address address) const;

		struct CacheLineEvictionEvent {
			explicit CacheLineEvictionEvent(Address evicted, Address admitted)
			: evictedAddress(evicted), admittedAddress(admitted) {}

			const Address evictedAddress;
			const Address admittedAddress;
		};

		observing::Subject<CacheLineEvictionEvent>& cacheLineEviction() { return cacheLineEviction_; }

	protected:
		virtual std::vector<Byte> readBlockImplementation(const Address address);
		virtual void writeBlockImplementation(const Address address, const std::vector<Byte> data);

	private:
		std::unique_ptr<eviction_policies::EvictionPolicy> evictionPolicy_;
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

/* OBSOLETE
#include "TypesAndUtility.h"
#include "CacheLine.h"
#include <vector>
#include <algorithm>
#include <cstdint>

namespace cache_simulation {
	class CacheLineSet {
	public:
		typedef std::vector<CacheLine>::iterator CacheLinePtr;
		typedef std::vector<CacheLine>::const_iterator CacheLineConstPtr;

		CacheLineSet() = delete;

		explicit CacheLineSet(const int size)
		: cacheLines(size)
		{}

		CacheLinePtr find(const Address address) {
			return std::find_if(cacheLines.begin(), cacheLines.end(),
				[address](const CacheLine& line) { return line.contains(address); }
			);
		}

		int size() const {
			return cacheLines.size();
		}

		CacheLine& operator[] (const int index) {
			return cacheLines[index];
		}

		const CacheLine& operator[] (const int index) const {
			return cacheLines[index];
		}

		const CacheLineConstPtr failure() const {
			return cacheLines.end();
		}

	private:
		std::vector<CacheLine> cacheLines;
	};
}
*/