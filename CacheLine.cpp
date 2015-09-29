#include "CacheLine.h"

namespace cache_simulation {
	/*
	std::vector<Byte> CacheLine::readBlockImplementation(const Address address) {
		if (address != tag()) {
			throw std::runtime_error("Tag and address don't match");
		}
	}

	void CacheLine::writeBlockImplementation(const Address address, const std::vector<Byte> data) {
		if (address != tag()) {
			throw std::runtime_error("Tag and address don't match");
		}

		setDirty(true);
		setValid(true);
	}*/

	void CacheLine::setDirty(const bool dirty) {
		addressAndValidity_ = dirty ?
			addressAndValidity_ | DIRTY_FLAG_BITMASK :
			addressAndValidity_ & ~DIRTY_FLAG_BITMASK;
	}

	void CacheLine::setValid(const bool valid) {
		addressAndValidity_ = valid ?
			addressAndValidity_ | VALID_FLAG_BITMASK :
			addressAndValidity_ & ~VALID_FLAG_BITMASK;
	}

	void CacheLine::setBlockAddress(const Address address) {
		addressAndValidity_ = (address             &  blockAddressBitMask())
						    | (addressAndValidity_ & ~blockAddressBitMask());
	}
}