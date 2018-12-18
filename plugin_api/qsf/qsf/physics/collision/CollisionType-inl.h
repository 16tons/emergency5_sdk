// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline CollisionType::CollisionType(uint32 id) :
		mId(id),
		mCollisionFilterGroup((getUninitialized<short>())),
		mCollisionMask(getUninitialized<short>())
	{
		// Nothing to do in here
	}

	inline CollisionType::~CollisionType()
	{
		// Nothing to do in here
	}

	inline uint32 CollisionType::getId() const
	{
		return mId;
	}

	inline const std::string& CollisionType::getName() const
	{
		return mName;
	}

	inline short CollisionType::getCollisionFilterGroup() const
	{
		return mCollisionFilterGroup;
	}

	inline short CollisionType::getCollisionMask() const
	{
		return mCollisionMask;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
