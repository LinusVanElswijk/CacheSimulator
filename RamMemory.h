#pragma once

#include "TypesAndUtility.h"
#include "MemoryView.h"
#include <unordered_map>
#include <vector>
#include <sstream>

namespace cache_simulation {
	class RamMemory final : public MemoryView {
	public:
		// Constructor/Destructor
		explicit RamMemory(const int blockSize) : MemoryView(blockSize) {}
		virtual ~RamMemory() {};

		virtual std::vector<Byte> readBlock(const Address address);
		virtual void writeBlock(const Address address, const std::vector<Byte> data);
		virtual bool contains(const Address address) const { return true; }

	private:
		void checkAddress(const Address address) const;
		void checkData(const std::vector<Byte> data) const;

		std::unordered_map<Address, std::vector<Byte>> blockMap_;
	};
}