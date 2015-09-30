#include "EvictionPolicy.h"

namespace cache_simulation {
namespace eviction_policies {
	namespace {
		class RandomEviction : public EvictionPolicy {
		public:
			virtual void onEvent(const BlockReadEvent& readEvent) { }
			virtual void onEvent(const BlockWriteEvent& writeEvent) { }
			virtual void onEvent(const CacheLineEvictionEvent& evictionEvent) { }

			virtual Address select(const std::vector<Address>& candidates) {
				return candidates[std::rand() % candidates.size()];
			}
		};

		class FifoEviction : public EvictionPolicy {
		public:
			virtual void onEvent(const BlockReadEvent& readEvent) {}

			virtual void onEvent(const BlockWriteEvent& writeEvent) {}

			virtual void onEvent(const CacheLineEvictionEvent& evictionEvent) {
				orderedAddresses_.remove(evictionEvent.evictedAddress);
				orderedAddresses_.push_front(evictionEvent.admittedAddress);
			}

			virtual Address select(const std::vector<Address>& candidates) {
				return orderedAddresses_.back();
			}

		private:
			std::list<Address> orderedAddresses_;
		};

		class RecentUseEviction : public EvictionPolicy {
		public:
			virtual void onEvent(const BlockReadEvent& readEvent) {
				moveToFront(readEvent.address);
			}

			virtual void onEvent(const BlockWriteEvent& writeEvent) {
				moveToFront(writeEvent.address);
			}

			virtual void onEvent(const CacheLineEvictionEvent& evictionEvent) {
				orderedAddresses_.remove(evictionEvent.evictedAddress);
				moveToFront(evictionEvent.admittedAddress);
			}

		protected:
			std::list<Address> orderedAddresses_;

		private:
			void moveToFront(const Address address) {
				orderedAddresses_.remove(address);
				orderedAddresses_.push_front(address);
			}
		};

		class LruEviction : public RecentUseEviction {
		public:
			virtual Address select(const std::vector<Address>& candidates) {
				return orderedAddresses_.back();
			}
		};

		class MruEviction : public RecentUseEviction {
		public:
			virtual Address select(const std::vector<Address>& candidates) {
				return orderedAddresses_.front();
			}
		};
	}


	std::shared_ptr<EvictionPolicy> instantiate(const EvictionPolicyType type) {
		switch (type)
		{
		case EvictionPolicyType::RANDOM:
			return std::shared_ptr<EvictionPolicy>(new RandomEviction());

		case EvictionPolicyType::FIFO:
			return std::shared_ptr<EvictionPolicy>(new FifoEviction());

		case EvictionPolicyType::LEAST_RECENTLY_USED:
			return std::shared_ptr<EvictionPolicy>(new LruEviction());

		case EvictionPolicyType::MOST_RECENTLY_USED:
			return std::shared_ptr<EvictionPolicy>(new MruEviction());

		default:
			throw std::runtime_error("No implementation for policy type");
		}
	}

}}