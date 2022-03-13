#pragma once
#include <functional>
namespace NCL
{
	namespace CSC8503
	{
		typedef std::function<void(float)> StateUpdateFunciton;
		class State
		{
		public:
			State(){}
			State(StateUpdateFunciton someFunc) {
				func = someFunc;
			}
			virtual ~State()
			{
			}

			void Update(float dt) {
				if (func != nullptr)
					func(dt);
			}
		protected:
			StateUpdateFunciton func;
		};	
	}
}
