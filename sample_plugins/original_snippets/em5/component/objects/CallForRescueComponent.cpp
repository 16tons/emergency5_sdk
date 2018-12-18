// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CallForRescueComponent::COMPONENT_ID = qsf::StringHash("em5::CallForRescueComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CallForRescueComponent::CallForRescueComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mCanRescuedByHelicopter(false),
		mCanRescuedByDLK(false)
	{
		// Nothing to do in here
	}

	CallForRescueComponent::~CallForRescueComponent()
	{
		// Nothing to do in here
	}

	bool CallForRescueComponent::getCanRescuedByHelicopter() const
	{
		return mCanRescuedByHelicopter;
	}

	void CallForRescueComponent::setCanRescuedByHelicopter(bool canRescuedByHelicopter)
	{
		mCanRescuedByHelicopter = canRescuedByHelicopter;

		if (isSimulating())
		{
			// Show icon over person
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_HELI, mCanRescuedByHelicopter);
		}
	}

	bool CallForRescueComponent::getCanRescuedByDLK() const
	{
		return mCanRescuedByDLK;
	}

	void CallForRescueComponent::setCanRescuedByDLK(bool canRescuedByDLK)
	{
		mCanRescuedByDLK = canRescuedByDLK;

		if (isSimulating())
		{
			// Show icon over person
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_DLK, mCanRescuedByDLK);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void CallForRescueComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mCanRescuedByHelicopter);
	}

	void CallForRescueComponent::onShutdown()
	{
		// Nothing here
	}

	void CallForRescueComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			if (mCanRescuedByHelicopter)
			{
				// Show icon over person
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_HELI, true);
			}
			else if (mCanRescuedByDLK)
			{
				// Show icon over person
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_DLK, true);
			}
		}
		else
		{
			// Hide icons
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_HELI, false);
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_DLK, false);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
