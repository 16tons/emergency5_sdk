// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/UpgradeComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"

#include <qsf/serialization/binary/StlTypeSerialization.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UpgradeComponent::COMPONENT_ID = qsf::StringHash("em5::UpgradeComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UpgradeComponent::UpgradeComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype)
	{
		// Nothing to do in here
	}

	UpgradeComponent::~UpgradeComponent()
	{
		// Nothing to do in here
	}

	std::string UpgradeComponent::getUpgradeId() const
	{
		return mUpgradeId;
	}

	void UpgradeComponent::setUpgradeId(const std::string& upgradeId)
	{
		mUpgradeId = upgradeId;
	}

	void UpgradeComponent::showUpgrade()
	{
		// Make upgrade entity visible
		getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
	}

	void UpgradeComponent::hideUpgrade()
	{
		// Make upgrade entity invisible
		getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::ZERO);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void UpgradeComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mUpgradeId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void UpgradeComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			getEntity().setHidden(true);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
