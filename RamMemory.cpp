#include "RamMemory.h"

namespace cache_simulation {

	std::vector<Byte> RamMemory::readBlockImplementation(const Address address) {
		checkAddress(address);
		return blockHandle(address)->second;
	}

	void RamMemory::writeBlockImplementation(const Address address, const std::vector<Byte>& data) {
		checkAddress(address);
		checkData(data);

		blockHandle(address)->second = data;
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

	RamMemory::BlockHandle RamMemory::blockHandle(const Address address) {
		auto blockHandle = blockMap_.find(address);
		if (blockHandle == blockMap_.end()) {
			std::pair<Address, std::vector<Byte>> newPair(address, std::vector<Byte>(blockSize()));
			blockHandle = blockMap_.insert(newPair).first;
		}

		return blockHandle;
	}
}