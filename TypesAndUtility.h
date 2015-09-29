#pragma once

#include <cstdint>
#include <memory>

namespace cache_simulation {
	typedef std::uintptr_t Address;
	typedef unsigned char Byte;

	inline bool isPowerOfTwo(const int x) {
		// Adapted from http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
		return (x > 0) && ((x & (x - 1)) == 0);
	}
}