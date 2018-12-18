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
		inline SnowComponent::SnowComponent(Prototype* prototype) :
			Component(prototype),
			mSnowAmount(1.0f),
			mMaterialChanged(false)
		{
			// Nothing to do
		}

		inline SnowComponent::~SnowComponent()
		{
			// Nothing to do in here
		}

		inline float SnowComponent::getSnowAmount() const
		{
			return mSnowAmount;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		inline bool SnowComponent::implementsOnComponentPropertyChange() const
		{
			// This component implements "onComponentPropertyChange()", so return "true"
			return true;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
