#pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {
		typedef std::function<void(float)> StateUpdateFunction;
		class State {
			public:
			State() {}
			State(StateUpdateFunction function) : stateFunction(function) {};
			virtual ~State() {};

			virtual void Update(float dt)
			{
				if (stateFunction)
					stateFunction(dt);
			};

		protected:
			StateUpdateFunciton stateFunction;
		};

		typedef void(*StateFunc)(void*);
		class GenericState : public State {
		public:
			GenericState(StateFunc someFunc, void* someData) {
				stateFunction = someFunc;
				funcData = someData;
			}
			virtual void Update(float dt) {
				if (funcData != nullptr) {
					func(funcData);
				}
			}
		protected:
			StateFunc stateFunction;
			void* funcData;
		};
	}
}