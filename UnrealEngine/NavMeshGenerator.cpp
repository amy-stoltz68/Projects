/******************************************************************************/
/*!
  \brief
	Procedurally Generates Navlinks from the verticles of each navmesh to 
	another
*/
/******************************************************************************/
void ATestGenerator::GenerateNavLinks()
{
	// gets all the verts of the navmesh
	GetVertices();

	UWorld *World = GetWorld(); // gets the world in order to spawn links
	FRotator rotation = FRotator::ZeroRotator; // will hold rotation of navlink

	if (World)
	{
		// goes through all vertices in mesh
		for (int32 i = 0; i < NavMeshVerts.Num(); ++i)
		{
			// creates position vector for each vert. Will be used for navlink spawn position
			FVector position{ NavMeshVerts[i].X, NavMeshVerts[i].Y, NavMeshVerts[i].Z };
			
			// goes through other verts to connect each to another
			for (int32 j = i; j < NavMeshVerts.Num(); ++j)
			{
				if (i == j)
					continue;

				// gets the "pointed to" link's position
				FVector pairPos{ NavMeshVerts[j].X, NavMeshVerts[j].Y, NavMeshVerts[j].Z };

				// gets the distance between each pair vertices
				float distance = FVector::Distance(position, pairPos);

				// if the vertices are too far apart, then discard navlink for these
				if (distance > MaxDistance)
					continue;

				FActorSpawnParameters SpawnParameters;

				// spawns navlink at the first vertex
				ANavLinkProxy *NavLink = World->SpawnActor<AMyNavLinkProxy>(BP_NavLink, position, rotation, SpawnParameters);
				
				// sets the left and right point links to the own and pair
				NavLink->PointLinks[0].Left = FVector::ZeroVector;
				NavLink->PointLinks[0].Right = NavMeshVerts[j] - position;

				// sets that smart link is relevent (for our AI pathing)
				NavLink->bSmartLinkIsRelevant = true;

				// gets the smart link comp and sets points traversable both ways
				UNavLinkCustomComponent *SmartLink = NavLink->GetSmartLinkComp();
				SmartLink->SetLinkData(NavLink->PointLinks[0].Left, NavLink->PointLinks[0].Right, ENavLinkDirection::BothWays);
				
				// gets rid of regular navlinks point links(we only want smart link)
				NavLink->PointLinks.Empty();

				//update the navmesh
				UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
				NavSystem->UpdateActorInNavOctree(*NavLink);

			}
		}
	}
}

/******************************************************************************/
/*!
  \brief
	Gets all the vertices from the entire navmesh
*/
/******************************************************************************/
TArray<FVector> ATestGenerator::GetVertices()
{
	// gets Navigation System and Nav Data
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();

	TArray<FVector> VerticesFinal; // will hold the positions for each vertex

	if (NavData)
	{
		 // get the navmesh
		ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);

		if (NavMesh)
		{
			TArray<FNavPoly> NavPolys; // will hold all of the polys in the mesh
			FBox TileBox;

			// goes through all the Navmesh Tiles
			for (int32 i = 0; i < NavMesh->GetNavMeshTilesCount(); ++i)
			{
				// gets the tile
				TileBox = NavMesh->GetNavMeshTileBounds(i);

				// if the tile isn't valid
				if (!TileBox.IsValid)
				{
					continue;
				}

				NavMesh->GetPolysInTile(i, NavPolys); // get the polygons in each tile
			}

			if (NavPolys.Num() <= 0)
				Print("WARNING: No Polys in Navmesh");
			
			
			FOccluderVertexArray Vertices; // holds the vertices contained in each poly

			// goes through the polygons
			for (int32 i = 0; i < NavPolys.Num(); ++i)
			{
				// gets the id and the verts from the corresponding poly
				uint32 id = NavMesh->GetPolyAreaID(NavPolys[i].Ref);
				FString ID = FString::FromInt(NavPolys[i].Ref);
				NavMesh->GetPolyVerts(NavPolys[i].Ref, Vertices);

				// goes through all collected vertices
				for (int j = 0; j < Vertices.Num(); ++j)
				{
					// gets position of each
					FVector position{ Vertices[j].X, Vertices[j].Y, Vertices[j].Z };

					// gets position and adjusts it slightly for error
					FVector dir = NavPolys[i].Center - position;
					dir.Normalize();
					position += dir * leeway;

					VerticesFinal.Add(position); // adds final position to array
				}
			}
		}
	}

	TSet<FVector> setVerts; // contain the positions into a set

	// adds the positions to the set
	for (int32 i = 0; i < VerticesFinal.Num(); ++i)
	{
		setVerts.Add(VerticesFinal[i]);
	}

	 // if it hasn't been filled, then put the final verts of the navmesh in the static array so we can get it later
	if(NavMeshVerts.Num() <= 0)
		NavMeshVerts.Append(setVerts.Array());

	return setVerts.Array(); // return the sorted positions

}
