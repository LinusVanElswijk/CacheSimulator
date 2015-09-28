#pragma once
/* OBSOLETE
#include "TypesAndUtility.h"
#include <vector>

namespace cache_simulation {
	class MemoryInterface {
	public:
		virtual ~MemoryInterface() {};

		virtual std::vector<Byte> readBytes(const Address address, const int nrOfBytes) = 0;
		virtual void writeBytes(const Address address, const std::vector<Byte> data) = 0;

		template<typename T>
		T read(const Address address);

		template<typename T>
		void write(const Address address, const T value);

		template<typename T>
		T read(T* const address);

		template<typename T>
		void write(T* const address, const T value);
	};

	template<typename T>
	T MemoryInterface::read(const Address address) {
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
	void MemoryInterface::write(const Address address, const T value) {
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
	T MemoryInterface::read(T* const address) {
		const Address castedAddress = reinterpret_cast<Address>(address);
		*address = read<T>(castedAddress);
		return *address;
	}

	template<typename T>
	void MemoryInterface::write(T* const address, const T value) {
		const Address castedAddress = reinterpret_cast<Address>(address);
		write<T>(castedAddress, value);
		*address = value;
	}	
}

*/