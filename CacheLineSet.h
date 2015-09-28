#pragma once
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