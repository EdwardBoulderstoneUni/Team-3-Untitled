#pragma once
#include <vector>
#include <map>

namespace NCL
{
	namespace CSC8503
	{
		class State;
		class StateTransition;

		using TransitionContainer = std::multimap<State*, StateTransition*>;
		using TransitionIterator = TransitionContainer::iterator;

		class StateMachine
		{
		public:
			StateMachine();
			~StateMachine();

			void AddState(State* s);
			void AddTransition(StateTransition* t);

			void Update(float dt);
		protected:
			State* activeState;

			std::vector<State*> allStates;

			TransitionContainer allTransitions;
		};
	}
}
