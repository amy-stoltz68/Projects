	namespace SpiderStates
	{
		/******************************************************************************/
		/*!
		   \brief
			  Returns whether there is a path between the spider and the player. 
			  Checks for empty tiles and tile height differences.
		*/
		/******************************************************************************/
		bool isHorizontalPath(Behavior *parent)
		{
			if (parent)
			{
        // gets spider and player object
				SpiderBehavior *spider_behav = getBehavior(parent);
				GameObject *player = getPlayerObj(parent);
        
        // if valid
				if (spider_behav && player)
				{
					// gets the ground object that the spider is on
					if (spider_behav->getGround())
					{
						// gets the tileMap
						TileMapCollider *groundTileMap = getTileMap(parent);
            
             // if valid
						if (groundTileMap)
						{
              // gets player and spider transforms for use
							Transform *spider_transform = getTransform(parent);
							Transform *player_transform = player->find_component<Transform>();
              
              // if valid
							if (spider_transform && player_transform)
							{
								// gets translation and collider of the spider and player for parameters of function
								glm::vec2 spider_translation = spider_transform->get_translation();
								glm::vec2 player_translation = player_transform->get_translation();
              
								RectangleCollider *spider_collider = getColliderRect(parent);
								RectangleCollider *player_collider = player->find_component<RectangleCollider>();

								//makes it so that the players y is the same as the spiders if level so that the comparison is accurate
								//player_translation.y += (spider_collider->get_scale().y - player_collider->get_scale().y) / 2.0f;

								if (spider_collider && player_collider)
								{
									// checks if there is a path between the player and spider without gaps or height difference
									if (groundTileMap->FindHorizontalPath(spider_translation, *spider_collider, player_translation, *player_collider))
									{
										// will also check that player is colliding with the ground so jumping doesn't cause problems
										if (spider_behav->collidingWithGround())
										{
											return true; // if all cases are true return that there is a valid path
										}
									}
								}
							}
						}
					}
				}
			}

			return false;
		}

		/******************************************************************************/
		/*!
		   \brief
			  Returns whether the spider will be in a wall or inbetween tiles at its
			  new position.
		*/
		/******************************************************************************/
		bool collidesAtNewPosition(Behavior *parent, glm::vec3 new_position)
		{
			if (parent)
			{
				SpiderBehavior *spider_behav = getBehavior(parent);

				if (spider_behav)
				{
          // gets the tiles the spider is on without gaps
					if (spider_behav->getGround())
					{
						// gets the tileMap
						TileMapCollider *groundTileMap = getTileMap(parent);

						if (groundTileMap)
						{
							RectangleCollider *spider_collider =getColliderRect(parent);

							if (spider_collider)
							{
								glm::vec2 spider_scale = spider_collider->get_scale();

								 // checks if there is a colision with the tiles around the spider
								if (groundTileMap->CheckScalePosForCollision(new_position, spider_scale))
								{
									return true; // if it will collide at the new position then return true
								}
							}
						}
					}
				}
			}

			return false; // it doesn't collide at the new position
		}

		/******************************************************************************/
		/*!
		   \brief
			  Returns whether the spider is at the end of its own platform.
		*/
		/******************************************************************************/
		bool endOfHome(Behavior *parent, glm::vec3 position)
		{
			if (parent)
			{
				SpiderBehavior *spider_behav = getBehavior(parent);

				if (spider_behav)
				{
          // gets the tiles that the spider is on without gaps
					if (spider_behav->getGround())
					{
						// gets the tileMap
						TileMapCollider *groundTileMap = getTileMap(parent);

						if (groundTileMap)
						{
							RectangleCollider *spider_collider = getColliderRect(parent);

							if (spider_collider)
							{
								// gets translations for direction vector
								glm::vec3 spider_translation = getTransform(parent)->get_translation();
								glm::vec3 player_translation = getPlayerObj(parent)->find_component<Transform>()->get_translation();

								glm::vec2 spider_scale = spider_collider->get_scale();
                
                // gets direction vector from spider to player position
								glm::vec2 direction_vec(player_translation.x - spider_translation.x, player_translation.y - spider_translation.y);
                
                // get the coordinates of the tiles that surround the spider
								TileMapCollider::ObjectTileCoords tile_struct = groundTileMap->GetObjectAsTileCoordinates(position, spider_scale);

								// if the player is to the left of the spider, then check the tile to the left of the spider(bottom left)
								if (direction_vec.x <= 0.0f)
								{
									// if this tile doesn't exist then return that its at the end
									if (!groundTileMap->Tile(tile_struct.BottomLeft.Left()))
									{
										return true; // the spider is at the end of the platform
									}
								}
								// if the player is to the right of the spider, then check the tile to the right of the spider(bottom right)
								else
								{
									// if this tile doesn't exist then return that its at the end
                  // checks to see if the there is an empty tile on the bottom right
									if (!groundTileMap->Tile(tile_struct.BottomRight.Right()))
									{
										return true;
									}


								}
							}
						}
					}
				}
			}
			return false; // it remains on the platform
		}

		/******************************************************************************/
		/*!
		   \brief
			  Returns whether the spider will collide with a wall while chasing the 
        player
		*/
		/******************************************************************************/
		bool collidingWithWall(Behavior *parent, glm::vec3 position)
		{
			if (parent)
			{
				if (parent)
				{
					SpiderBehavior *spider_behav = getBehavior(parent);

					if (spider_behav)
					{
            // gets the ground the spider is currently on
						if (spider_behav->getGround())
						{
							// gets the tileMap
							TileMapCollider *groundTileMap = getTileMap(parent);

							RectangleCollider *spider_collider = getColliderRect(parent);

							if (spider_collider)
							{
								// gets tranlations for direction vector
								glm::vec3 spider_translation = getTransform(parent)->get_translation();
								glm::vec3 player_translation = getPlayerObj(parent)->find_component<Transform>()->get_translation();

								glm::vec2 spider_scale = spider_collider->get_scale();
                
                // gets direction vector from spider to player
								glm::vec2 direction_vec(player_translation.x - spider_translation.x, player_translation.y - spider_translation.y);
                
                // gets tiles surrounding spider
								TileMapCollider::ObjectTileCoords tile_struct = groundTileMap->GetObjectAsTileCoordinates(position, spider_scale);

								// if the spider is going to the left, if there is a wall to the left then no go
								if (direction_vec.x <= 0.0f)
								{
									if (groundTileMap->Tile(tile_struct.BottomLeft.Up()))
									{
										return true;
									}
								}
								// if the spider is going to the right, if there is wall to the right then don't hit the wall
								else
								{
									if (groundTileMap->Tile(tile_struct.BottomRight.Up()))
									{
										return true;
									}
								}
							}
						}
					}
				}
			}

			return false; // the spider won't hit the wall
		}
