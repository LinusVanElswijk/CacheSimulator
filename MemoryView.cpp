#include "MemoryView.h"

namespace cache_simulation {

	MemoryView::MemoryView(const int blockSize)
	: blockSize_(blockSize)
	, blockReading_()
	, blockWriting_() {
		if (!(isPowerOfTwo(blockSize) && blockSize >= MINIMUM_BLOCK_SIZE)) {
			throw std::runtime_error("BlockSize should be a power of 2 larger or equal to " + MINIMUM_BLOCK_SIZE);
		}
	}

	std::vector<Byte> MemoryView::readBlock(const Address address) {
		blockReading_.notifyObservers(address, contains(address));
		return readBlockImplementation(address);
	}

	void MemoryView::writeBlock(const Address address, const std::vector<Byte> data) {
		blockWriting_.notifyObservers(address, data, contains(address));
		writeBlockImplementation(address, data);
	}

	std::vector<Byte> MemoryView::readBytes(const Address address, const int nrOfBytes) {
		checkDataAccessValidity(address, nrOfBytes);

		const Address blockAddress = toBlockAddress(address);
		const Address offset = toOffsetInBlock(address);

	// function body
		const std::vector<Byte> block = readBlock(blockAddress);
		std::vector<Byte> bytes(nrOfBytes);

		for (int i = 0; i < nrOfBytes; i++) {
			bytes[i] = block[i + offset];
		}

		return bytes;
	}

	void MemoryView::writeBytes(const Address address, const std::vector<Byte> data) {
		checkDataAccessValidity(address, static_cast<int>(data.size()));

		const Address blockAddress = toBlockAddress(address);
		const Address offset = toOffsetInBlock(address);

		if (data.size() == blockSize()) {
			writeBlock(blockAddress, data);
		}
		else {
			std::vector<Byte> block = readBlock(blockAddress);
			
			for (unsigned int i = 0; i < data.size(); i++) {
				block[i + offset] = data[i];
			}

			if (offset != 0) {
				int x = 4;
			}

			writeBlock(blockAddress, block);
			auto block2 = readBlock(blockAddress);

			for (int i = 0; i < block2.size(); i++) {
				if (block2[i] != block[i]) {
					int x = 5;
				}

			}
		}
	}

	void MemoryView::checkDataAccessValidity(const Address address, const int byteCount) const {
		if (byteCount > blockSize()) {
			std::stringstream message;
			message << "Trying to read/write " << byteCount << " bytes, but blocksize is only " << blockSize() << "." << std::ends;
			throw new std::runtime_error(message.str());
		}
		
		const int offset = static_cast<int>(toOffsetInBlock(address));

		if (byteCount + offset > blockSize()) {
			std::stringstream message;
			message << "Trying to read/write " << byteCount << " bytes at address " << address << ", (offset: " << offset <<
				"), but this would exceed the blocksize (" << blockSize() << ")." << std::ends;
			throw new std::runtime_error(message.str());
		}
	}
}