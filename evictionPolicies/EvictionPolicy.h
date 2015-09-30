#pragma once

#include "../observing/Observer.h"
#include "../Events.h"
#include <algorithm>
#include <list>

namespace cache_simulation {
namespace eviction_policies {

	enum class EvictionPolicyType {
		RANDOM,
		FIFO,
		LEAST_RECENTLY_USED,
		MOST_RECENTLY_USED
	};

	class EvictionPolicy
	: public observing::Observer<BlockWriteEvent>
	, public observing::Observer<BlockReadEvent>
	, public observing::Observer<CacheLineEvictionEvent> {
	public:
		EvictionPolicy()
		: observing::Observer<BlockWriteEvent>()
		, observing::Observer<BlockReadEvent>()
		, observing::Observer<CacheLineEvictionEvent>()
		{}

		virtual ~EvictionPolicy() {};

		virtual Address select(const std::vector<Address>& candidates) = 0;
	};

	std::shared_ptr<EvictionPolicy> instantiate(const EvictionPolicyType type);
}}

