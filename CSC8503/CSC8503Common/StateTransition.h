#pragma once
#include <functional>
namespace NCL
{
	namespace CSC8503
	{
		class State;
		typedef std::function<bool()> StateTransitionFunction;
		class StateTransition
		{
		public:
			StateTransition(State* source, State* dest, StateTransitionFunction f) {
				sourceState = source;
				destinationState = dest;
				funcition = f;
			}
			bool CanTransition() const {
				return funcition();
			}
			State* GetDestinationState() const { return destinationState; }
			State* GetSourceState() const { return sourceState; }
		protected:
			State* sourceState;
			State* destinationState;
			StateTransitionFunction funcition;
		};
	}
}
