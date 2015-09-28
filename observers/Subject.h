#pragma once

#include "Observer.h"
#include <list>
#include <memory>

namespace cache_simulation {
namespace observers {

	template<typename E>
	class Subject {
	private:
		typedef Observer<E> Observer;

	public:
		Subject() : observers_() {}

		void attach(std::shared_ptr<Observer> observer) {
			detach(observer);
			observers_.emplace_back(observer);
		}

		void detach(std::shared_ptr<Observer> observer) {
			observers_.remove_if(
				[observer](const std::weak_ptr<Observer>& weak) {
					return weak.expired() || (weak.lock() == observer);
				}
			);
		}

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

	private:
		Subject(const Subject& source) = delete;
		Subject& operator= (const Subject& source) = delete;

		std::list< std::weak_ptr<Observer> > observers_;
	};

}}