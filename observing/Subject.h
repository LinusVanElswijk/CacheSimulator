#pragma once

#include "Observer.h"
#include <list>
#include <memory>

namespace cache_simulation {
namespace observing {

	template<typename E>
	class Subject {
	private:
		typedef Observer<E> Observer;

	public:
		Subject() : observers_() {}
		virtual ~Subject() {};

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

	protected:
		std::list< std::weak_ptr<Observer> > observers_;
	};

}}