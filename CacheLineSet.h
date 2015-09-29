#pragma once

#include "MemoryCache.h"
#include "CacheLine.h"
#include <algorithm>

namespace cache_simulation {
	class CacheLine;

	class CacheLineSet: public MemoryCache {
	public:
		CacheLineSet(const int setSize, const int blockSize, MemoryView& upstream) : MemoryCache(blockSize, upstream) {}
		virtual ~CacheLineSet() {}

		int size() const {
			return cacheLines_.size();
		}

		virtual bool contains(const Address address) const;

	protected:
		virtual std::vector<Byte> readBlockImplementation(const Address address) = 0;
		virtual void writeBlockImplementation(const Address address, const std::vector<Byte> data) = 0;

	private:
		std::vector<CacheLine> cacheLines_;
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