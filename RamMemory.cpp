#include "RamMemory.h"

namespace cache_simulation {

	std::vector<Byte> RamMemory::readBlockImplementation(const Address address) {
		checkAddress(address);

		const auto blockHandle = blockMap_.find(address);
		if (blockHandle == blockMap_.end()) {
			return std::vector<Byte>(blockSize());
		}

		return blockHandle->second;
	}

	void RamMemory::writeBlockImplementation(const Address address, const std::vector<Byte> data) {
		checkData(data);
		blockMap_.emplace(address, data);
	}

	void RamMemory::checkAddress(const Address address) const {
		const Address offset = toOffsetInBlock(address);

		if (offset != 0) {
			std::stringstream message;
			message << "Requested address (" << address << ") is not aligned to a block address (offset: " << offset << ")." << std::ends;
			throw new std::runtime_error(message.str());
		}
	}

	void RamMemory::checkData(const std::vector<Byte> data) const {
		if (data.size() != blockSize()) {
			std::stringstream message;
			message << "Data size (" << data.size() << ") is not equal to the block size (" << blockSize() << ")." << std::ends;
			throw new std::runtime_error(message.str());
		}
	}
}