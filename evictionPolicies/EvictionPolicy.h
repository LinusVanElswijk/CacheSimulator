#pragma once

#include "../observing/Observer.h"
#include "../MemoryView.h"
#include <algorithm>

namespace cache_simulation {
namespace eviction_policies {

	enum class EvictionPolicyType {
		RANDOM
	};

	class EvictionPolicy
	: public observing::Observer<MemoryView::BlockWriteEvent>
	, public observing::Observer<MemoryView::BlockReadEvent> {
	public:
		EvictionPolicy()
		: observing::Observer<MemoryView::BlockWriteEvent>()
		, observing::Observer<MemoryView::BlockReadEvent>()
		{}

		virtual ~EvictionPolicy() {};

		virtual Address select(std::vector<Address> candidates) {
			Address best = candidates.front();
			
			for (auto it = candidates.begin()++; it != candidates.end(); it++) {
				best = compare(best, *it) ? best : *it;
			}

			return best;
			/*
			const auto best = std::min_element(begin(candidates), end(candidates), [this](const Address left, const Address right) {
				return this->compare(left, right);
			});
			
			return *best;
			*/
		}
		
		virtual bool compare(const Address left, const Address right) = 0;
	};

	class RandomEviction : public EvictionPolicy {
	public:
		virtual void onEvent(const MemoryView::BlockReadEvent& readEvent) { }
		virtual void onEvent(const MemoryView::BlockWriteEvent& writeEvent) { }

		virtual Address select(std::vector<Address> candidates) {
			return candidates[std::rand() % candidates.size()];
		}

		virtual bool compare(const Address left, const Address right) {
			return (std::rand() % 2) == 0;
		}
	};
}}

