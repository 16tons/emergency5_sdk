// Copyright (C) 2012-2017 Promotion Software GmbH

//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf_ai/reflection/ExportedProperty.h>

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ActionComponent;
	namespace logic
	{
		class TargetPoint;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		* @brief A component to trigger a random movement every time the entity is idle.
		* A movement towards a static random goal position is triggered every time the entity is found idle.
		* To avoid a performance impact in case the entity is trapped and pathfinding fails often there is a configurable delay before starting to search again.
		* This component requires the entity to have an qsf::ActionPlan and a an qsf::ai::Navigation component attached to work.
		* Each component is updated by the job manager individually.
		* The goals are being defined in 2D with the height position taken from the terrain height at that position.
		*/
		class QSF_GAME_API_EXPORT RandomMovementComponent : public qsf::Component
		{
		public:
			static const uint32 COMPONENT_ID;

			// Min and max corners for the area where the goal is selected randomly
			static const qsf::ai::ExportedProperty<glm::vec2> MIN_GOAL_CORNER;
			static const qsf::ai::ExportedProperty<glm::vec2> MAX_GOAL_CORNER;

			// Use the map boundaries as goal corners, this invalidates the manual boundaries set
			static const qsf::ai::ExportedProperty<bool> USE_MAP_BOUNDARIES_AS_GOAL_AREA;

			// Min and max bounds for a time window when to check again whether the entity is idle and a new goal is needed
			static const qsf::ai::ExportedProperty<Time> MIN_UPDATE_CHECK_DELAY;
			static const qsf::ai::ExportedProperty<Time> MAX_UPDATE_CHECK_DELAY;

			// The action priority to use when pushing move actions from inside this component
			static const qsf::ai::ExportedProperty<uint32> ACTION_PRIORITY;

			explicit RandomMovementComponent(qsf::Prototype* prototype);

			const glm::vec2& getMinGoalCorner() const;
			void setMinGoalCorner(const glm::vec2& corner);

			const glm::vec2& getMaxGoalCorner() const;
			void setMaxGoalCorner(const glm::vec2& corner);

			Time getMinUpdateCheckDelay() const;
			void setMinUpdateCheckDelay(Time minDelay);

			Time getMaxUpdateCheckDelay() const;
			void setMaxUpdateCheckDelay(Time maxDelay);

			bool usesMapBoundariesAsGoalArea() const;
			void setUsesMapBoundariesAsGoalArea(bool enable);

			uint32 getActionPriority() const;
			void setActionPriority(uint32 priority);

			/**
			*  @brief
			*    Check if the simulation is allowed
			*  @brief
			*    Return "true" if the simulation is allowed
			*/
			virtual bool onUpdateSimulation();


		protected:
			// protected component interface implementation
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;

			virtual void onSetSimulating(bool simulating) override;
			virtual void onSetActive(bool active) override;
			//@}

			virtual void pushMoveAction(qsf::ActionComponent& actionComponent, qsf::logic::TargetPoint& moveTargetPoint);


		private:
			// Main update function that is passed to the job manager
			void updateJob(const qsf::JobArguments& jobArguments);

			// The corners of the area where to find goals in.
			glm::vec2 mMinGoalCorner;
			glm::vec2 mMaxGoalCorner;

			// A window for the minimal and maximal delay before trying to define a new goal again.
			// These should be unsigned because a negative value makes no sense
			Time mMinUpdateCheckDelay;
			Time mMaxUpdateCheckDelay;

			// A resulting absolute earliest time for the next check based on the update delay settings
			Time mEarliestNextCheck;

			// Use the map boundaries as goal corners, this invalidates the manual boundaries set.
			// This is only evaluated once during startup and overwrites the manual settings if set to true.
			bool mUsesMapBoundariesAsGoalArea;

			// The priority of the actions pushed onto the plan.
			uint32 mPriority;

			qsf::JobProxy mUpdateJobProxy;

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline RandomMovementComponent::RandomMovementComponent(qsf::Prototype* prototype) :
			Component(prototype),
			mMinGoalCorner(MIN_GOAL_CORNER.mDefault),
			mMaxGoalCorner(MAX_GOAL_CORNER.mDefault),
			mMinUpdateCheckDelay(MIN_UPDATE_CHECK_DELAY.mDefault),
			mMaxUpdateCheckDelay(MAX_UPDATE_CHECK_DELAY.mDefault),
			mUsesMapBoundariesAsGoalArea(USE_MAP_BOUNDARIES_AS_GOAL_AREA.mDefault),
			mPriority(ACTION_PRIORITY.mDefault)
		{}

		inline const glm::vec2& RandomMovementComponent::getMinGoalCorner() const
		{
			return mMinGoalCorner;
		}

		inline void RandomMovementComponent::setMinGoalCorner(const glm::vec2& corner)
		{
			assignAndPromoteChange(mMinGoalCorner, corner, MIN_GOAL_CORNER.mId);
		}

		inline const glm::vec2& RandomMovementComponent::getMaxGoalCorner() const
		{
			return mMaxGoalCorner;
		}

		inline void RandomMovementComponent::setMaxGoalCorner(const glm::vec2& corner)
		{
			assignAndPromoteChange(mMaxGoalCorner, corner, MAX_GOAL_CORNER.mId);
		}

		inline Time RandomMovementComponent::getMinUpdateCheckDelay() const
		{
			return mMinUpdateCheckDelay;
		}

		inline void RandomMovementComponent::setMinUpdateCheckDelay(Time minDelay)
		{
			assignAndPromoteChange(mMinUpdateCheckDelay, minDelay, MIN_UPDATE_CHECK_DELAY.mId);
		}

		inline Time RandomMovementComponent::getMaxUpdateCheckDelay() const
		{
			return mMaxUpdateCheckDelay;
		}

		inline void RandomMovementComponent::setMaxUpdateCheckDelay(Time maxDelay)
		{
			assignAndPromoteChange(mMaxUpdateCheckDelay, maxDelay, MAX_UPDATE_CHECK_DELAY.mId);
		}

		inline bool RandomMovementComponent::usesMapBoundariesAsGoalArea() const
		{
			return mUsesMapBoundariesAsGoalArea;
		}

		inline void RandomMovementComponent::setUsesMapBoundariesAsGoalArea(bool enable)
		{
			assignAndPromoteChange(mUsesMapBoundariesAsGoalArea, enable, USE_MAP_BOUNDARIES_AS_GOAL_AREA.mId);
		}

		inline uint32 RandomMovementComponent::getActionPriority() const
		{
			return mPriority;
		}

		inline void RandomMovementComponent::setActionPriority(uint32 priority)
		{
			assignAndPromoteChange(mPriority, priority, ACTION_PRIORITY.mId);
		}
	}
}

QSF_CAMP_TYPE(qsf::game::RandomMovementComponent);
