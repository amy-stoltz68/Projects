namespace Gam200
{
	namespace PlayerStates
	{
		 // helper function that sets jump/fall speeds and animations for fall and jump state
		static void jump_fall_calc(Behavior *parent);
		
	  /******************************************************************************/
	  /*!
	    \brief
		  Instantiates the Idle State. Singleton use.
	  */
	  /******************************************************************************/
		Idle* Idle::instance()
		{
			static Idle instance;

			return &instance;
		}
    
    /******************************************************************************/
	  /*!
	    \brief
		  On Enter for Player Idle
      
      \param parent
      Player behavior component
	  */
	  /******************************************************************************/
		void Idle::Enter(Behavior *parent)
		{
      // gets the player animation and rigid body
			AnimationSpine* ani = getAniSp(parent);
			RigidBody *rigidbod =getRB(parent);
			
      // player stands still
			rigidbod->set_velocityX(0.0f);
      
      // if Lily dies, then play death animation
			if (getBehavior(parent)->getIsDead())
			{
				ani->set_speed(3.0f); // changes animation speed

				ani->play("G1_Anim_DeathL_01", false); // plays the ani
			}
      // if she isn't dead then just play idle animation
			else
			{
        // plays idle animation
				ani->set_speed(1.0f);
				ani->play("G1_Anim_IdleL_01", true);
			}

		}
		
    /******************************************************************************/
	  /*!
	    \brief
		  Update for Player Idle
      
      \param parent
      Player behavior component
	  */
	  /******************************************************************************/
		void Idle::Execute(Behavior *parent)
		{
      // gets necessary comps
			PlayerBehavior *player_behav = getBehavior(parent);
			StateMachine *SM = getSM(parent);
			RigidBody *rigidbod = getRB(parent);
      
       // if player isn't dead
			if (!player_behav->getIsDead())
			{
        
        // check for player input
				if (CheckKey(player_behav->get_key(Keys::LEFT)) || CheckKey(player_behav->get_key(Keys::RIGHT)))
				{
					SM->changeState(Walk::instance()); // walks left or write
				}

				if (CheckKeyPressed(player_behav->get_key(Keys::JUMP)))
				{
					SM->changeState(Jump::instance()); // jumps up
				}

				if (CheckKeyPressed(player_behav->get_key(Keys::BURN)))
				{
					SM->changeState(Burn::instance()); // burns web
				}

				if (rigidbod && SM && rigidbod->get_velocity().y < 0)
				{
					SM->changeState(Fall::instance()); // is falling
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /******************************************************************************/
	  /*!
	    \brief
		  Instantiates the Walk State. Singleton use.
	  */
	  /******************************************************************************/
		Walk* Walk::instance()
		{
			static Walk instance;
			return &instance;
		}
    
    /******************************************************************************/
	  /*!
	    \brief
		  On Enter for Player Walk State.
      
      \param parent
      Player behavior component.
	  */
	  /******************************************************************************/
		void Walk::Enter(Behavior *parent)
		{
      // gets animation
			AnimationSpine* ani = getAniSp(parent);

			if (ani)
			{
        // plays walk ani
				ani->play("G1_Anim_RunL_01", true);
				ani->set_speed(1.0f);
			}
		}
    
    /******************************************************************************/
	  /*!
	    \brief
		  Update for Player Walk
      
      \param parent
      Player behavior component
	  */
	  /******************************************************************************/
		void Walk::Execute(Behavior *parent)
		{
      // gets necessary components
			AnimationSpine* walk_ani = getAniSp(parent);
			RigidBody *rigidbod = getRB(parent);
			PlayerBehavior *player = getBehavior(parent);
			StateMachine *SM = getSM(parent); // gets state machine
			
      // gets the player's current velocity vector
			glm::vec2 velocity = rigidbod->get_velocity();

			if (walk_ani && rigidbod && player)
			{
				 // if the player is walking off something her state goes to fall
				if (rigidbod->get_velocity().y < 0)
				{
					SM->changeState(Fall::instance());
				}
				
         // checks for input: Whether the ani gets flipped based on controls
				if (CheckKey(player->get_key(Keys::RIGHT)))
				{
					walk_ani->set_flipX(false);
					velocity.x = player->getSpeed();
				}
				
				if (CheckKey(player->get_key(Keys::LEFT)))
				{

					walk_ani->set_flipX(true);
					velocity.x = -player->getSpeed();
				}
        
        // makes player not move if both directions are pressed
				if(!CheckKey(player->get_key(Keys::RIGHT)) && !CheckKey(player->get_key(Keys::LEFT)))
				{
					velocity.x = 0.0f;
					SM->changeState(Idle::instance());
				}
        
         // if up button pressed, then jump
				if (CheckKeyPressed(player->get_key(Keys::JUMP)))
				{
					SM->changeState(Jump::instance());
				}

				rigidbod->set_velocityX(velocity.x);
			}
		}
