// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const Time& QsfOgreEntity::getLastRenderedTimestamp() const
	{
		return (mLastRenderedTimestamp[0] > mLastRenderedTimestamp[1]) ? mLastRenderedTimestamp[0] : mLastRenderedTimestamp[1];
	}

	inline const Time& QsfOgreEntity::getLastNormalRenderedTimestamp() const
	{
		return mLastRenderedTimestamp[0];
	}

	inline const Time& QsfOgreEntity::getLastShadowRenderedTimestamp() const
	{
		return mLastRenderedTimestamp[1];
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline QsfOgreEntity::QsfOgreEntity(Ogre::IdType id, Ogre::ObjectMemoryManager* objectMemoryManager, Ogre::ObjectMemoryManager* lodObjectMemoryManager, Ogre::SceneManager* manager) :
		Ogre::v1::Entity(id, objectMemoryManager, lodObjectMemoryManager, manager),
		mIsCustomBoundingBoxSet(false)
	{
		// Nothing to do in here
	}

	inline QsfOgreEntity::QsfOgreEntity(Ogre::IdType id, Ogre::ObjectMemoryManager* objectMemoryManager, Ogre::ObjectMemoryManager* lodObjectMemoryManager, Ogre::SceneManager* manager, const Ogre::v1::MeshPtr& mesh) :
		Ogre::v1::Entity(id, objectMemoryManager, lodObjectMemoryManager, manager, mesh),
		mIsCustomBoundingBoxSet(false)
	{
		// Nothing to do in here
	}

	inline QsfOgreEntity::~QsfOgreEntity()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
