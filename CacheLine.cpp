#include "CacheLine.h"

namespace cache_simulation {

	std::vector<Byte> CacheLine::readBlockImplementation(const Address address) {
		if (address != blockAddress()) {
			throw std::runtime_error("Address doesn't match block address");
		}

		return data_;
	}

	void CacheLine::writeBlockImplementation(const Address address, const std::vector<Byte>& data) {
		if (address != blockAddress()) {
			throw std::runtime_error("Address doesn't match block address");
		}

		if (data.size() != blockSize()) {
			throw std::runtime_error("Data size doesn't match block size");
		}

		data_ = data;
	}

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