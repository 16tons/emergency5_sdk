// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/escape/EscapeCarLongAction.h"
#include "em5/action/move/escape/EscapeCarShortAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/EscapeSpecsGroup.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EscapeCarLongAction::ACTION_ID = "em5::EscapeCarLongAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EscapeCarLongAction::EscapeCarLongAction() :
		EscapeAction(ACTION_ID),
		mElapsedTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	EscapeCarLongAction::~EscapeCarLongAction()
	{
		// Nothing here
	}

	void EscapeCarLongAction::init(const std::string& escapeTargetTag)
	{
		mEscapeTargetTag = escapeTargetTag;

		glm::vec3 bestPoint;
		{
			qsf::Entity* targetEntity = getTargetEntity();
			if (nullptr != targetEntity)
			{
				bestPoint = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
			}
			else
			{
				const qsf::TransformComponent& selfTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
				const glm::vec3& selfPosition = selfTransformComponent.getPosition();
				const float escapeTimeInSeconds = EscapeSpecsGroup::getInstanceSafe().getEscapeTimeCarLong();
				mMaxTime = qsf::Time::fromSeconds(escapeTimeInSeconds);
				const float moveDistance = escapeTimeInSeconds * 5.0f;
				const float startAngle = qsf::Random::getRandomFloat(0.0f, 6.28f);
				for (int i = 0; i < 4; ++i)
				{
					// We begin with a random direction, then try adding 90°, then 180°, then 270° (in radians, of course)
					const float angle = startAngle + i * 1.57f;
					const glm::vec3 newPoint = selfPosition + glm::vec3(glm::sin(angle), 0, glm::cos(angle)) * moveDistance;

					bestPoint = newPoint;

					if (MapHelper(getEntity().getMap()).isInMapBoundaries(bestPoint))
					{
						// Found a suitable point, early out
						break;
					}
				}
			}
		}

		qsf::logic::TargetPoint moveTargetPoint;
		moveTargetPoint.mPosition = bestPoint;

		// Call base class implementation
		qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_RUN, nullptr);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void EscapeCarLongAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		serializer.serialize(mElapsedTime);
		serializer.serialize(mMaxTime);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeCarLongAction::updateAction(const qsf::Clock& clock)
	{
		if (!checkAllowUpdate())
		{
			return qsf::action::RESULT_CONTINUE;
		}

		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		if (qsf::action::RESULT_CLEARLIST == result)
			result = qsf::action::RESULT_DONE;

		mElapsedTime += clock.getTimePassed();
		if (mElapsedTime > mMaxTime)
			result = qsf::action::RESULT_DONE;

		if (result == qsf::action::RESULT_DONE)
		{
			qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.pushAction<EscapeCarShortAction>(action::COMMAND_STD, qsf::action::APPEND_TO_BACK).init(mEscapeTargetTag);
		}

		return result;
	}

	void EscapeCarLongAction::onShutdown()
	{
		// Call base implementation
		EscapeAction::onShutdown();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
