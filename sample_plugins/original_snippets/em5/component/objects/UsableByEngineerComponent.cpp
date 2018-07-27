// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UsableByEngineerComponent::COMPONENT_ID = qsf::StringHash("em5::UsableByEngineerComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UsableByEngineerComponent::UsableByEngineerComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mPlacement(HIGH),
		mUseTime(qsf::Time::fromSeconds(5.0f)),
		mUsed(false),
		mIsInUse(false)
	{
		// Nothing to do in here
	}

	UsableByEngineerComponent::~UsableByEngineerComponent()
	{
		// Nothing to do in here
	}

	void UsableByEngineerComponent::initWithUseTime(qsf::Time useTime)
	{
		setUseTime(useTime);
		setIsInUse(false);
		setUsed(false);

		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT, true);
	}

	UsableByEngineerComponent::Placement UsableByEngineerComponent::getPlacement() const
	{
		return mPlacement;
	}

	void UsableByEngineerComponent::setPlacement(Placement placement)
	{
		mPlacement = placement;
	}

	qsf::Time UsableByEngineerComponent::getUseTime() const
	{
		return mUseTime;
	}

	void UsableByEngineerComponent::setUseTime(qsf::Time useTime)
	{
		mUseTime = useTime;
	}

	float UsableByEngineerComponent::getUseTimeInSeconds() const
	{
		return mUseTime.getSeconds();
	}

	void UsableByEngineerComponent::setUseTimeInSeconds(float useTime)
	{
		mUseTime.setSeconds(useTime);
	}

	bool UsableByEngineerComponent::getUsed() const
	{
		return mUsed;
	}

	void UsableByEngineerComponent::setUsed(bool wasUsed)
	{
		mUsed = wasUsed;
	}

	bool UsableByEngineerComponent::getIsInUse() const
	{
		return mIsInUse;
	}

	void UsableByEngineerComponent::setIsInUse(bool isInUse)
	{
		mIsInUse = isInUse;
	}

	void UsableByEngineerComponent::cleanup()
	{
		mUsed = false;
		mIsInUse = false;
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT, false);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void UsableByEngineerComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mPlacement);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
