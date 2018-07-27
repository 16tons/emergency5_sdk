// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Entity::~Entity()
	{
		// Destroy all components within this prototype
		// -> Do not remove this just because it's already within the prototype destructor
		destroyAllComponents();
	}

	inline bool Entity::isSerializable() const
	{
		return mFlags.isSet(SERIALIZABLE);
	}

	inline void Entity::setSerializable(bool serializable)
	{
		mFlags.set(SERIALIZABLE, serializable);
	}

	inline bool Entity::hasSimulationTransformListeners() const
	{
		return !mSimulationTransformListeners.empty();
	}

	inline bool Entity::hasAnimationTransformListeners() const
	{
		return !mAnimationTransformListeners.empty();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
