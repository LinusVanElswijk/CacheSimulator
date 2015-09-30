#pragma once

#include "TypesAndUtility.h";
#include <vector>

namespace cache_simulation {
	struct BlockReadEvent {
		BlockReadEvent(const Address address, const bool hit)
		:address(address), cacheHit(hit) {}

		const Address address;
		const bool cacheHit;
	};

	struct BlockWriteEvent {
		BlockWriteEvent(const Address address, const std::vector<Byte> data, const bool hit)
		:address(address), cacheHit(hit) {}

		const Address address;
		const std::vector<Byte> data;
		const bool cacheHit;
	};

	struct CacheLineEvictionEvent {
		explicit CacheLineEvictionEvent(Address evicted, Address admitted)
		: evictedAddress(evicted), admittedAddress(admitted) {}

		const Address evictedAddress;
		const Address admittedAddress;
	};
}