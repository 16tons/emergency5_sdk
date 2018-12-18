// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuryComponent::COMPONENT_ID	= qsf::StringHash("em5::BuryComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuryComponent::~BuryComponent()
	{
		// Nothing here
	}

	void BuryComponent::unlinkAndShowBuriedEntities()
	{
		qsf::Map& map = getEntity().getMap();

		for (uint64 entityId : mBuriedEntities)
		{
			qsf::Entity* buriedEntity = map.getEntityById(entityId);
			if (nullptr != buriedEntity)
			{
				// Destroy the linkage
				qsf::LinkComponent* linkComponent = buriedEntity->getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					linkComponent->unlinkFromParent();
				}

				// Unhide the entity
				buriedEntity->destroyComponent<qsf::game::HiddenComponent>();
				AnimationHelper(*buriedEntity).clearAnimation();

				qsf::MessageParameters parameters;
				parameters.setParameter("DebrisId", getEntityId());
				parameters.setParameter("UnburiedId", buriedEntity->getId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_UNHIDE_BURIED, getEntityId(), buriedEntity->getId()), parameters);
			}
		}

		// Forget all the buried entities
		mBuriedEntities.clear();
	}

	bool BuryComponent::isBlocked() const
	{
		return mIsBlocked;
	}

	void BuryComponent::setBlocked(bool blocked)
	{
		mIsBlocked = blocked;
	}

	void BuryComponent::buryEntity(qsf::Entity& entity)
	{
		qsf::LinkComponent& linkComponent = entity.getOrCreateComponentSafe<qsf::LinkComponent>();
		linkComponent.setParentId(getEntityId());
		BuriedEntitiesArray.insert(0, entity.getId());
		setBuriedEntityHiddenType(entity);
	}

	void BuryComponent::showCuttableBySawIcon(bool enable)
	{
		// This adds/removes both status icon and mini-map icon
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_TREE, enable);

		mIsHidden = !enable;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void BuryComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mIsHidden);
		serializer.serialize(mBuriedEntities);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void BuryComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			hideBuriedEntities();
		}
		else
		{
			showBuriedEntities();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BuryComponent::showBuriedEntities()
	{
		qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mBuriedEntities)
		{
			qsf::Entity* buriedEntity = map.getEntityById(entityId);
			if (nullptr != buriedEntity)
			{
				buriedEntity->destroyComponent<qsf::game::HiddenComponent>();
				AnimationHelper(*buriedEntity).clearAnimation();
			}
		}
	}

	void BuryComponent::hideBuriedEntities()
	{
		qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mBuriedEntities)
		{
			qsf::Entity* buriedEntity = map.getEntityById(entityId);
			if (nullptr != buriedEntity)
			{
				setBuriedEntityHiddenType(*buriedEntity);
			}
		}
	}

	void BuryComponent::setBuriedEntityHiddenType(qsf::Entity& buriedEntity) const
	{
		// Remember we enter the visibility change in the constructor (set invisible) and shortly after this in the setHiddenType() function
		qsf::game::HiddenComponent* hiddenComponent = buriedEntity.getOrCreateComponent<qsf::game::HiddenComponent>();
		if (nullptr != hiddenComponent)
		{
			hiddenComponent->setHiddenType((mIsHidden ? qsf::game::HiddenComponent::HIDDENTYPE_BLOCKED : qsf::game::HiddenComponent::HIDDENTYPE_BLOCKED_BUT_VISIBLE));
			AnimationHelper(buriedEntity).clearAnimation();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
