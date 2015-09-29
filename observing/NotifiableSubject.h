#pragma once

#include "Subject.h"

namespace cache_simulation {
namespace observing {

	template<typename E>
	class NotifiableSubject : public Subject<E> {
	public:
		NotifiableSubject() : Subject() {}
		virtual ~NotifiableSubject() {};
			
		void notifyObservers(const E& e) {
			for (auto it = observers_.begin(); it != observers_.end(); it++) {
				auto observer = it->lock();
				if (observer) {
					observer->onEvent(e);
				}
				else {
					observers_.erase(it);
				}
			}
		}

		template <typename... TArgs>
		void notifyObservers(const TArgs&... args) { notifyObservers(E(args...)); }
	};
}
}