#pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {
		typedef std::function<void(float)> StateUpdateFunction;
		class State {
		public:
			State() {};
			State(StateUpdateFunction func)
				: function(func)
			{};
			virtual ~State() {};

			virtual void Update(float dt)
			{
				if (function)
					function(dt);
			};

		protected:
			StateUpdateFunction function;
		};

		typedef void(*StateFunc)(void*);

		class GenericState : public State {
		public:
			GenericState(StateFunc someFunc, void* someData) {
				func = someFunc;
				funcData = someData;
			}
			virtual void Update(float dt) {
				if (funcData != nullptr) {
					func(funcData);
				}
			}
		protected:
			StateFunc func;
			void* funcData;
		};
	}
}