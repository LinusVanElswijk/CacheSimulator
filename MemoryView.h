#pragma once

#include "TypesAndUtility.h"

#include "observers\Subject.h"
#include "observers\Observer.h"

#include <vector>
#include <sstream>

namespace cache_simulation {
	class MemoryView 
	{
	public:
		// Constructor/Destructor
		explicit MemoryView(const int blockSize) : blockSize_(blockSize), blockReading(), blockWriting() {}
		virtual ~MemoryView() {};

		// What subclasses need to implement
		virtual std::vector<Byte> readBlock(const Address address) = 0;
		virtual void writeBlock(const Address address, const std::vector<Byte> data) = 0;
		virtual bool contains(const Address address) const = 0;

		// Accessors	
		int blockSize() const { return blockSize_; }

		// Read write operations
		std::vector<Byte> readBytes(const Address address, const int nrOfBytes);
		void writeBytes(const Address address, const std::vector<Byte> data);

		template<typename T>
		T read(const Address address);

		template<typename T>
		void write(const Address address, const T value);

		template<typename T>
		T read(T* const address);

		template<typename T>
		void write(T* const address, const T value);

		// Events
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

		observers::Subject<BlockReadEvent> blockReading;
		observers::Subject<BlockWriteEvent> blockWriting;

	protected:
		Address offsetBitmask() const { return (blockSize() - 1); }
		Address blockAddressBitMask() const { return ~offsetBitmask(); }
		Address toOffsetInBlock(const Address address) const { return address & offsetBitmask(); }
		Address toBlockAddress(const Address address) const { return address & blockAddressBitMask(); }

	private:
		void checkDataAccessValidity(const Address address, const int byteCount) const;

		const int blockSize_;
	};

	// implementation of template functions
	template<typename T>
	T MemoryView::read(const Address address) {
		const static int byteCount = sizeof(T);
		const std::vector<Byte> bytes = readBytes(address, byteCount);

		T value = 0;
		value |= bytes[bytes.size() - 1];
		for (int i = bytes.size() - 2; i >= 0; i--) {
			value <<= 8;
			value |= bytes[i];
		}

		return value;
	}

	template<typename T>
	void MemoryView::write(const Address address, const T value) {
		const static int byteCount = sizeof(T);
		std::vector<Byte> bytes(byteCount);

		T copy = value;
		for (unsigned int i = 0; i < bytes.size(); i++) {
			bytes[i] = copy & 0xFF;
			copy >>= 8;
		}

		writeBytes(address, bytes);
	}

	template<typename T>
	T MemoryView::read(T* const address) {
		const Address castedAddress = reinterpret_cast<Address>(address);
		*address = read<T>(castedAddress);
		return *address;
	}

	template<typename T>
	void MemoryView::write(T* const address, const T value) {
		const Address castedAddress = reinterpret_cast<Address>(address);
		write<T>(castedAddress, value);
		*address = value;
	}
}