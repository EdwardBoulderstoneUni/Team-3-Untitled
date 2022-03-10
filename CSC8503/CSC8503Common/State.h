#pragma once

namespace NCL
{
	namespace CSC8503
	{
		class State
		{
		public:
			State()
			{
			}

			virtual ~State()
			{
			}

			virtual void Update() = 0; //Pure virtual base class
		};

		using StateFunc = void(*)(void*);

		class GenericState : public State
		{
		public:
			GenericState(StateFunc someFunc, void* someData)
			{
				func = someFunc;
				funcData = someData;
			}

			void Update() override
			{
				if (funcData != nullptr)
				{
					func(funcData);
				}
			}

		protected:
			StateFunc func;
			void* funcData;
		};
	}
}
