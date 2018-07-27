// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GameLogic::~GameLogic()
	{
		// Nothing here
	}

	inline uint32 GameLogic::getId() const
	{
		return mId;
	}

	inline uint32 GameLogic::getTypeId() const
	{
		return mTypeId;
	}

	inline uint64 GameLogic::getEntityId() const
	{
		return mEntityId;
	}

	inline bool GameLogic::isRunning() const
	{
		return mFlags.isSet(RUNNING);
	}

	inline bool GameLogic::isDebug() const
	{
		return mFlags.isSet(DEBUG);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	inline bool GameLogic::onStartup()
	{
		// Default implementation: simply return success
		return true;
	}

	inline void GameLogic::onShutdown()
	{
		// Default implementation: do nothing
	}

	inline void GameLogic::serialize(BinarySerializer& serializer)
	{
		// Default implementation: do nothing
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::GameLogic methods              ]
	//[-------------------------------------------------------]
	inline void GameLogic::onSetDebug(bool)
	{
		// Default implementation: do nothing
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline GameLogic::GameLogic(uint32 typeId) :
		mManager(nullptr),
		mId(getUninitialized<uint32>()),
		mTypeId(typeId),
		mEntityId(0)
	{
		// Nothing here
	}

	inline void GameLogic::registrationAtManager(GameLogicManager& manager, uint32 id)
	{
		mManager = &manager;
		mId = id;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
