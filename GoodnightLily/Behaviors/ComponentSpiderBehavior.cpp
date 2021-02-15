/******************************************************************************/
	/*!
	  \brief
		Updates the spider behavior.
	*/
	/******************************************************************************/
	void SpiderBehavior::update(float dt)
	{
		// gets original first placement spider position (does it in update because in init the transform isnt initialized
		if (!doOnce)
		{
			if (get_sibling("Transform"))
			{
				spider_original_x = getTransform(this)->get_translation().x;
				spider_original_y = getTransform(this)->get_translation().y;

				doOnce = true;
			}
		}

		// updates all the spider states
		stateMachine->update(dt);


		// check for all of the sounds
		sound_check();

		// sets that spider will attack Lily
		if (get_distance_between(player) <= dist_to_attack)
		{
			isWithinRangeAttack = true;
			isWithinRangeChase = false;
		}
		// spider will chase Lily
		else if (get_distance_between(player) <= dist_to_chase)
		{
			isWithinRangeChase = true;
			isWithinRangeAttack = false;
		}
		// spider does nothing :(
		else
		{
			isWithinRangeChase = false;
			isWithinRangeAttack = false;
		}

		// if the spider is attacking, and its at the top of its attack pose
		if (stateMachine->getCurrentState() == SpiderStates::Attack::instance())
		{
			if (getAniSp(this))
			{
        
        			// if the spider is beginning to attack
				if (getAniSp(this)->find_event("S1_Event_Attack"))
				{
					// play the attack audio
					AudioMessage::Shout(AudioAction::aud_Play_Event, "event:/SFX/enemy/Spider_swipe");

					// if the player is in the danger zone then she respawns(awakens)
					if (get_distance_between(player) <= dist_to_respawn)
					{
						AnimationSpine* Ani = getAniSp(this); // make sure the animation stops so this remains E10
						Ani->set_speed(0.0f);
						isTriggeredRespawn = true; // Lily now respawns
					}
				}
			}
		}

		// if the spider is burrowed start decrementing the timer for the spider to stay underground
		if (stateMachine->getCurrentState() == SpiderStates::Burrow::instance())
		{
			wait_timer -= dt;
		}

		sendEventData(); // send the respawn data to the player(whether she is dead or not)

		isTriggeredRespawn = false; // resets it to false every frame
		playerIsColliding = false;
	}
