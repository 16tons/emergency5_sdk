// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MarkerComponent::MarkerComponent(Prototype* prototype) :
		Component(prototype),
		mSpeed(1.0f),
		mOgreEntity(nullptr),
		mTime(0.0f),
		mVisible(true),
		mPreviousActive(true)
	{
		// Nothing to do in here
	}

	inline MarkerComponent::~MarkerComponent()
	{
		// Nothing to do in here
	}

	inline float MarkerComponent::getSpeed() const
	{
		return mSpeed;
	}

	inline void MarkerComponent::setSpeed(float speed)
	{
		assignAndPromoteChange(mSpeed, speed, SPEED);
	}

	inline Ogre::v1::Entity* MarkerComponent::getOgreEntity() const
	{
		return mOgreEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
