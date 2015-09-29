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

		virtual bool contains(const Address address) const { return true; }

	protected:
		virtual std::vector<Byte> readBlockImplementation(const Address address);
		virtual void writeBlockImplementation(const Address address, const std::vector<Byte> data);

	private:
		void checkAddress(const Address address) const;
		void checkData(const std::vector<Byte> data) const;

		std::unordered_map<Address, std::vector<Byte>> blockMap_;
	};
}