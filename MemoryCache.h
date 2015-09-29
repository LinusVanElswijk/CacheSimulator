#pragma once

#include "MemoryView.h"

namespace cache_simulation {

	class MemoryCache : public MemoryView {
	public:
		explicit MemoryCache(const int blockSize, MemoryView& upstream) : MemoryView(blockSize), upstream_(upstream) {
			if (upstream_.blockSize() != blockSize) {
				throw std::runtime_error("Upstream blocksize mismatch.");
			}
		}
		virtual ~MemoryCache() {};
		
	protected:
		MemoryView& upstream_;

	private:
		//MemoryCache() = delete;
		//MemoryCache(const MemoryCache& source) = delete;
		//MemoryCache& operator=(const MemoryCache& source) = delete;
	};
}