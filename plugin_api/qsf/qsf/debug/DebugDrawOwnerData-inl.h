// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DebugDrawOwnerData::DebugDrawOwnerData() :
		mComponent(nullptr)
	{
		// Nothing to do in here
	}

	inline DebugDrawOwnerData::DebugDrawOwnerData(Component* component) :
		mComponent(component)
	{
		// Nothing to do in here
	}

	inline Component* DebugDrawOwnerData::getComponent() const
	{
		return mComponent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
