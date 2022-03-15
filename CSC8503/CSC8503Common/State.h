#pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {
		typedef std::function<void(float)> StateUpdateFunction;
		class State {
			public:
			State() {}
			State(StateUpdateFunction function) : stateFunction(function) {}
			virtual ~State() {}

			virtual void Update(float dt)
			{
				if (stateFunction != nullptr)
					stateFunction(dt);
			}

		protected:
			StateUpdateFunction stateFunction;
		};
	}
}