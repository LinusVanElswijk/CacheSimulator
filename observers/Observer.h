#pragma once

namespace cache_simulation {
namespace observers {

	template<typename TEvent>
	class Observer {
	public:
		virtual void onEvent(const TEvent& e) = 0;
	};

}
}