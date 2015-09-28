#pragma once
/* OBSOLETE
#include "MemoryInterface.h"

namespace cache_simulation {

	class CacheInterface : public MemoryInterface {
	public:
		CacheInterface() = delete;

		explicit CacheInterface(MemoryInterface& upstream)
		: upstream(upstream)
		{}

		virtual std::vector<Byte> readBytes(const Address address, const int nrOfBytes) = 0;
		virtual void writeBytes(const Address address, const std::vector<Byte> data) = 0;

		virtual ~CacheInterface() {};

	protected:
		MemoryInterface& upstream;
	};
}
*/