#pragma once

namespace cache_simulation {
namespace observing {

	template<typename E>
	class Observer {
	public:
		virtual ~Observer() {};
		virtual void onEvent(const E& e) = 0;
	};

}
}