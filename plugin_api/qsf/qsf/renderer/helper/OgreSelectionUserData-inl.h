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
	inline OgreSelectionUserData::OgreSelectionUserData() :
		mComponent(nullptr),
		mDebugDrawId(getUninitialized<uint32>())
	{
		// Nothing here
	}

	inline OgreSelectionUserData::OgreSelectionUserData(Component* component) :
		mComponent(component),
		mDebugDrawId(getUninitialized<uint32>())
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Overloading of operators                              ]
	//[-------------------------------------------------------]
	inline std::ostream& operator<<(std::ostream& out, OgreSelectionUserData& userData)
	{
		out << userData.mComponent << userData.mDebugDrawId;
		return out;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
