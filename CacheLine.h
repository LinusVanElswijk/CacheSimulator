#pragma once 

#include "MemoryView.h"

namespace cache_simulation {

	struct CacheLine : public MemoryView {
	public:
		explicit CacheLine(const int blockSize) : MemoryView(blockSize), addressAndValidity_(0), data_(blockSize) {}
		virtual ~CacheLine() {}

		virtual bool contains(const Address address) const { return blockAddress() == address && isValid(); }

		bool isDirty() const { return (addressAndValidity_ & DIRTY_FLAG_BITMASK) != 0; }
		bool isValid() const { return (addressAndValidity_ & VALID_FLAG_BITMASK) != 0; }
		Address blockAddress() const { return toBlockAddress(addressAndValidity_); }
		std::vector<Byte> data() { return readBlock(blockAddress()); }

		void setDirty(const bool dirty);
		void setValid(const bool valid);
		void setBlockAddress(const Address address);
		void setData(const std::vector<Byte> data) { writeBlock(blockAddress(), data); }

	protected:
		virtual std::vector<Byte> readBlockImplementation(const Address address);
		virtual void writeBlockImplementation(const Address address, const std::vector<Byte> data);

	private:
		const static Address VALID_FLAG_BITMASK = 1 << 0;
		const static Address DIRTY_FLAG_BITMASK = 1 << 1;

		Address addressAndValidity_;
		std::vector<Byte> data_;
	};
}

/* OBSOLETE
#include "TypesAndUtility.h"
#include "MemoryInterface.h"
#include <stdexcept>

namespace cache_simulation {

	struct CacheLine : public MemoryInterface {
	public:
		static_assert(sizeof(Address) == sizeof(int *), "Address type size should match the size of int*");

	private:
		const static Address VALID_FLAG_BITMASK = 1 << 0;
		const static Address DIRTY_FLAG_BITMASK = 1 << 1;

	public:
		const static int MINIMUM_BLOCK_SIZE = 4;

		CacheLine(const int blockSize)
		: tagAndValidity_(0)
		, data_(blockSize)
		{	
			if (!(isPowerOfTwo(blockSize) && blockSize >= MINIMUM_BLOCK_SIZE)) {
				throw std::runtime_error("BlockSize should be a power of 2 larger or equal to " + MINIMUM_BLOCK_SIZE);
			}
		}

		int blockSize() const { return data_.size(); }

		bool isDirty() const { return (tagAndValidity_ & DIRTY_FLAG_BITMASK) != 0; }
		bool isValid() const { return (tagAndValidity_ & VALID_FLAG_BITMASK) != 0; }
		Address tag() const { return (tagAndValidity_ & tagBitmask()); }

		virtual std::vector<Byte> readBytes(const Address address, const int nrOfBytes) {
			const Address offset = address & offsetBitmask();

			std::vector<Byte> bytes(nrOfBytes);
			for (int i = 0; i < nrOfBytes; i++) {
				bytes.emplace_back(data_[i + offset]);
			}
			return bytes;
		};
		
		virtual void writeBytes(const Address address, const std::vector<Byte> data) {
			const Address offset = address & offsetBitmask();
			const int nrOfBytes = data.size();

			std::vector<Byte> bytes(nrOfBytes);
			for (int i = 0; i < nrOfBytes; i++) {
				data_[i + offset] = bytes[i];
			}
		};

		const std::vector<Byte>& rawData() const {
			return data_;
		}

		template<typename T>
		T dataAt(Address address) const {
			if (!contains(address)) { throw std::runtime_error("CacheLine does not contain the specified address."); }

			const static byteCount = sizeof(T);
			const Address offset = address & offsetBitmask();

			T t = 0;
			t |= data[0];
			for (int i = 0; i < byteCount; i++) {
				t <<= 8;
				t |= data[i + offset];
			}

			return t;
		}

		bool contains(const Address address) const {
			return isValid() && ((address & tagBitmask()) == tag());
		}

		void setDirty(const bool dirty) {
			tagAndValidity_ = dirty ?
				tagAndValidity_ | DIRTY_FLAG_BITMASK :
				tagAndValidity_ & ~DIRTY_FLAG_BITMASK;
		}

		void setValid(const bool valid) {
			tagAndValidity_ = valid ?
				tagAndValidity_ | VALID_FLAG_BITMASK :
				tagAndValidity_ & ~VALID_FLAG_BITMASK;
		}

		void setTag(const Address address) {
			tagAndValidity_ = (address         &  tagBitmask())
							| (tagAndValidity_ & ~tagBitmask());
		}

		template<typename T>
		void setDataAt(const Address address, T value) const {
			if (!contains(address)) { throw std::runtime_error("CacheLine does not contain the specified address."); }

			const static byteCount = sizeof(T);
			const Address offset = address & offsetBitmask();

			for (int i = 0; i < byteCount; i++) {
				data[i + offset] = reinterpret_cast<Byte>(value);
				t >>= 8;
			}

			return t;
		}

	private:
		Address tagAndValidity_;
		std::vector<Byte> data_;

		Address offsetBitmask() const { return (blockSize() << 1) - 1; }
		Address tagBitmask() const { return ~offsetBitmask(); }
	};
}
*/