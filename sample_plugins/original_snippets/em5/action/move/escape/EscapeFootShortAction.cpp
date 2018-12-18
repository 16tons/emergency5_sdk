// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/EscapeSpecsGroup.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EscapeFootShortAction::ACTION_ID = "em5::EscapeFootShortAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EscapeFootShortAction::EscapeFootShortAction() :
		EscapeAction(ACTION_ID)
	{
		// Nothing here
	}

	EscapeFootShortAction::~EscapeFootShortAction()
	{
		// Nothing here
	}

	void EscapeFootShortAction::catched()
	{
		mElapsedTime = mPhaseEndTime;
	}

	void EscapeFootShortAction::init(const std::string& escapeTargetTag)
	{
		bool targetPositionSet = false;

		mEscapeTargetTag = escapeTargetTag;
		if (!mEscapeTargetTag.empty())
		{
			qsf::Entity* targetEntity = getTargetEntity();
			if (nullptr != targetEntity)
			{
				mTargetPosition = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
				targetPositionSet = true;
			}
		}

		if (!targetPositionSet)
		{
			mTargetPosition = getClosestTargetPoint(true);
			if (qsf::isUninitialized(mTargetPosition))
				mTargetPosition = getClosestTargetPoint(false);
		}

		qsf::logic::TargetPoint moveTargetPoint(mTargetPosition, false, 0.0f, 2.0f);

		const EscapeSpecsGroup& specsGroup = EscapeSpecsGroup::getInstanceSafe();
		mPhaseEndTime = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getRunTimeFootShortMin(), specsGroup.getRunTimeFootShortMax()));

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_STARTED, ACTION_ID));

		// Call base class implementation
		qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_RUN, nullptr);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void EscapeFootShortAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		serializer.serialize(mElapsedTime);
		serializer.serialize(mPhaseEndTime);
		serializer.serialize(mTargetPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeFootShortAction::updateAction(const qsf::Clock& clock)
	{
		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		if (qsf::action::RESULT_CLEARLIST == result)
		{
			result = qsf::action::RESULT_DONE;
		}

		mElapsedTime += clock.getTimePassed();

		if (mElapsedTime > mPhaseEndTime)
		{
			const EscapeSpecsGroup& specsGroup = EscapeSpecsGroup::getInstanceSafe();
			mPhaseEndTime = mElapsedTime + qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getRunTimeFootLongMin(), specsGroup.getRunTimeFootLongMax()));

			qsf::Time breakTime = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getBreatheTimeFootLongMin(), specsGroup.getBreatheTimeFootLongMax()));
			pushBreathAnimation(breakTime);

			// TODO(mk) Here is a very good place to change the movement position. To avoid bugs in path finding we should not move outside the map or to "strange" places
		}

		EntityHelper entityHelper(getEntity());
		const bool isInMapBoundaries = entityHelper.isInMapBoundaries();

		if (result == qsf::action::RESULT_DONE || !isInMapBoundaries)
		{
			const glm::vec3& currentPos = getEntity().getComponent<qsf::TransformComponent>()->getPosition();

			const bool closeToDespawnPoint = (glm::distance2(currentPos, mTargetPosition) < 5.0f);

			// The gangster escapes and the respective entity is destroyed when the gangster is close to
			// map boundaries or the desired despawn point
			if (closeToDespawnPoint || !isInMapBoundaries || entityHelper.isCloseToMapBoundaries())
			{
				// Send message
				qsf::MessageParameters parameters;
				parameters.setParameter("GangsterId", getEntityId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_ESCAPED, getEntityId()), parameters);

				// Destroy the entity
				MapHelper::destroyEntity(getEntity());
				return qsf::action::RESULT_CLEARLIST;
			}
		}

		return result;
	}

	void EscapeFootShortAction::onShutdown()
	{
		// Call base implementation
		EscapeAction::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	glm::vec3 EscapeFootShortAction::getClosestTargetPoint(bool onlyMapExits)
	{
		std::vector<PersonSpawnPointComponent*> exitPersonSpawnPoints;

		// Sort the vector for entry and exit
		for (PersonSpawnPointComponent* personSpawnPointComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<PersonSpawnPointComponent>())
		{
			// Test if we have the right gate
			if (personSpawnPointComponent->isActive() && personSpawnPointComponent->isExit())
			{
				// onlyMapExits -> !isInBoundaries
				if (!onlyMapExits || !EntityHelper(personSpawnPointComponent->getEntity()).isInMapBoundaries())
					exitPersonSpawnPoints.push_back(personSpawnPointComponent);
			}
		}

		if (exitPersonSpawnPoints.empty())
			return qsf::getUninitialized<glm::vec3>();

		const qsf::TransformComponent& selfTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
		glm::vec3 selfPosition = selfTransformComponent.getPosition();
		float shortestSquaredDistance = std::numeric_limits<float>::max();
		glm::vec3 position;

		for (PersonSpawnPointComponent* personSpawnPointComponent : exitPersonSpawnPoints)
		{
			const qsf::TransformComponent& spawnPointTransformComponent = personSpawnPointComponent->getEntity().getComponentSafe<qsf::TransformComponent>();

			const glm::vec3& pointPosition = spawnPointTransformComponent.getPosition();
			const float squaredDistance = glm::distance2(pointPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				shortestSquaredDistance = squaredDistance;
				position = pointPosition;
			}
		}

		return position;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
