// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline MaterialFloatAccessComponent::MaterialFloatAccessComponent(Prototype* prototype) :
			MaterialAccessComponent(prototype),
			mFloatPropertyValue(0.0f)
		{
			// Nothing here
		}

		inline MaterialFloatAccessComponent::~MaterialFloatAccessComponent()
		{
			// Nothing to do in here
		}

		inline float MaterialFloatAccessComponent::getFloatPropertyValue() const
		{
			return mFloatPropertyValue;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
