/******************************************************************************/
/*!
\file   States.h
\author Amy Stoltz
\date   2/2020
\brief
	This file contains the functions for the states.

	© 2020 DigiPen, All Rights Reserved.
*/
/******************************************************************************/

#ifndef STATE_H
#define STATE_H
#include "ComponentBehavior.h"


#define getAniSp(parent) parent->get_sibling<AnimationSpine>()
#define getAniSeq(parent) parent->get_sibling<AnimationSequence>()
#define getRB(parent) parent->get_sibling<RigidBody>()
#define getTransform(parent) parent->get_sibling<Transform>()
#define getSpriteSp(parent) parent->get_sibling<SpriteSpine>()
#define getSpriteSeq(parent) parent->get_sibling<SpriteSequence>()
#define getColliderRect(parent) parent->get_sibling<RectangleCollider>()

namespace Gam200
{	
	class State
	{
	public:
		virtual void Enter(Behavior*) = 0;  // what happens when it first enters the state
		virtual void Execute(Behavior*) = 0; // the main event of the state
		virtual void Exit(Behavior*) = 0; // what happens when it exits the state
		virtual ~State() {}; // deconstructor
	};

	class StateMachine
	{
	public:
		// default constructor
		StateMachine(Behavior *parent);

		// sets the current and previous states
		void setCurrentState(State *state);
		void setPrevState(State *state);

		//calls the execute for each state
		void update(float dt);

		// changes the states to either next or previous
		void changeState(State *newState);
		void returnToPrevState();

		// getters
		State *getCurrentState();
		State *getPrevState();

	private:
		Behavior *parent_behavior; // the behavior that will have the state machine
		State *current_state; // the current state the behavior is in
		State *prev_state; // the previous state the behavior is in
	};
}
#endif // !STATE
