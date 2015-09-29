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

		Address select(std::vector<Address> candidates) {
			auto compareFunction = [this](const Address left, const Address right){ return this->compare(left, right); };
			std::sort(candidates.begin(), candidates.end(), compareFunction);
			return candidates.front();
		}
		
		virtual bool compare(const Address left, const Address right) = 0;
	};

	class RandomEviction : public EvictionPolicy {
	public:
		virtual void onEvent(const MemoryView::BlockReadEvent& readEvent) { }
		virtual void onEvent(const MemoryView::BlockWriteEvent& writeEvent) { }

		virtual bool compare(const Address left, const Address right) {
			return (std::rand() % 2) == 0;
		}
	};
}}

