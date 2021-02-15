/******************************************************************************/
/*!
\file   States.cpp
\author Amy Stoltz
\date   2/2020
\brief
	This file contains the functions for the state machine.

	© 2020 DigiPen, All Rights Reserved.
*/
/******************************************************************************/

#include "State.h"
namespace Gam200
{
	// constructor for the state machine
	StateMachine::StateMachine(Behavior *parent) :parent_behavior(parent), current_state(nullptr), prev_state(nullptr)
	{}

	// sets the state
	void StateMachine::setCurrentState(State *state)
	{
		current_state = state;
	}

	// sets the previous state
	void StateMachine::setPrevState(State *state)
	{
		prev_state = state;
	}

  // executes the behavior
	void StateMachine::update(float dt)
	{
		if (current_state)
		{
			current_state->Execute(parent_behavior);
		}
	}
  
  // changes the state to designated new state
	void StateMachine::changeState(State *newState)
	{
		if (newState)
		{
			prev_state = current_state; // updates previous

			current_state->Exit(parent_behavior); // calls the exit on the current
			current_state = newState; // updates the current to the new state
			current_state->Enter(parent_behavior); // calls the enter for the new current state
		}
	}
  
  // changes current state back to previous
	void StateMachine::returnToPrevState()
	{
		changeState(prev_state);
	}

  // gets the current state of the state machine
	State *StateMachine::getCurrentState()
	{
		return current_state;
	}
  
  // gets the previous state of the state machine
	State *StateMachine::getPrevState()
	{
		return prev_state;
	}
}
