// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/game/animation/AnimationHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SupportLegsComponent::COMPONENT_ID = qsf::StringHash("em5::SupportLegsComponent");


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SupportLegsComponent::startExtending()
	{
		// Anything to do?
		if (mSupportLegsState == SUPPORTLEGSSTATE_EXTENDING_ANIMATION || mSupportLegsState == SUPPORTLEGSSTATE_EXTENDED)
			return;

		// Change state
		mSupportLegsState = SUPPORTLEGSSTATE_EXTENDING_ANIMATION;

		AudioHelper::playCraneExtendLegs(mAudioProxy, getEntity());

		// Start animation for all support legs
		qsf::AssetProxy animationAssetProxy = mAnimationExtend.isValid() ? mAnimationExtend : qsf::AssetProxy(AnimationHelper(getEntity()).getAnimationSupportLegExtend());
		qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mSupportLegIds)
		{
			qsf::Entity* supportLegEntity = map.getEntityById(entityId);
			if (nullptr != supportLegEntity)
			{
				AnimationHelper animationHelper(*supportLegEntity);
				animationHelper.playAnimation(animationAssetProxy, false, false);
			}
		}

		// Activate job so we get regular update calls
		activateJob();
	}

	void SupportLegsComponent::hideExtending()
	{
		// Anything to do?
		if (mSupportLegsState == SUPPORTLEGSSTATE_HIDDING_ANIMATION || mSupportLegsState == SUPPORTLEGSSTATE_HIDDEN)
			return;

		// Change state
		mSupportLegsState = SUPPORTLEGSSTATE_HIDDING_ANIMATION;

		AudioHelper::playCraneHideLegs(mAudioProxy, getEntity());

		// Start animation for all support legs
		qsf::AssetProxy animationAssetProxy = mAnimationExtend.isValid() ? mAnimationExtend : qsf::AssetProxy(AnimationHelper(getEntity()).getAnimationSupportLegExtend());
		qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mSupportLegIds)
		{
			qsf::Entity* supportLegEntity = map.getEntityById(entityId);
			if (nullptr != supportLegEntity)
			{
				AnimationHelper animationHelper(*supportLegEntity);
				animationHelper.playAnimation(animationAssetProxy, false, true);
			}
		}

		// Deactivate job
		deactivateJob();
	}

	bool SupportLegsComponent::isAnimationPlaying()
	{
		if (mSupportLegsState == SUPPORTLEGSSTATE_EXTENDING_ANIMATION || mSupportLegsState == SUPPORTLEGSSTATE_HIDDING_ANIMATION)
		{
			// Start animation for all support legs
			qsf::Map& map = getEntity().getMap();
			for (uint64 entityId : mSupportLegIds)
			{
				qsf::Entity* supportLegEntity = map.getEntityById(entityId);
				if (nullptr != supportLegEntity)
				{
					qsf::ActionComponent* actionComponent = supportLegEntity->getComponent<qsf::ActionComponent>();
					if (nullptr != actionComponent && nullptr != actionComponent->getCurrentAction())
					{
						// Currently the play animation action is not evaluated, count this case as "animation is playing"
						continue;
					}

					qsf::MeshAnimationComponent* meshAnimationComponent = supportLegEntity->getComponent<qsf::MeshAnimationComponent>();
					if (nullptr != meshAnimationComponent && meshAnimationComponent->isPlaying())
					{
						continue;
					}
				}

				// One animation is finish, return false
				return false;
			}

			// All animation are playing, return true
			return true;
		}

		return false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void SupportLegsComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mSupportLegsState);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SupportLegsComponent::activateJob()
	{
		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&SupportLegsComponent::updateJob, this, _1));
		}
	}

	void SupportLegsComponent::deactivateJob()
	{
		mAudioProxy.stop();
		mAudioProxy.synchronizeWithMultiplay();
		mUpdateJobProxy.unregister();
	}

	void SupportLegsComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		switch (mSupportLegsState)
		{
			case SUPPORTLEGSSTATE_EXTENDING_ANIMATION:
			{
				if (!isAnimationPlaying())
				{
					mSupportLegsState = SUPPORTLEGSSTATE_EXTENDED;
					deactivateJob();
				}
				break;
			}

			case SUPPORTLEGSSTATE_HIDDING_ANIMATION:
			{
				if (!isAnimationPlaying())
				{
					mSupportLegsState = SUPPORTLEGSSTATE_HIDDEN;
					deactivateJob();
				}
				break;
			}

			case SUPPORTLEGSSTATE_EXTENDED:
			case SUPPORTLEGSSTATE_HIDDEN:
				// Nothing to do in this case
				break;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
