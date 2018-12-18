// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsGroundMapComponent::PhysicsGroundMapComponent(Prototype* prototype) :
		PhysicsComponent(prototype),
		mGroundMapId(1)
	{
		// Nothing to do in here
	}

	inline PhysicsGroundMapComponent::~PhysicsGroundMapComponent()
	{
		// Nothing to do in here
	}

	inline uint32 PhysicsGroundMapComponent::getGroundMapId() const
	{
		return mGroundMapId;
	}

	inline void PhysicsGroundMapComponent::setGroundMapId(uint32 groundMapId)
	{
		if (mGroundMapId != groundMapId)
		{
			mGroundMapId = groundMapId;
			promotePropertyChange(GROUNDMAPID_PROPERTY_ID);

			if (isRunning())
			{
				rebuildGroundMapTiles();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
