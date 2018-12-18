// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/trafficControl/DynamicNavigationBlockerComponent.h"
#include "qsf_ai/reflection/ExportedProperty.h"

namespace qsf
{
	namespace ai
	{
		/**
		* A simple barrier component defines the behavior of a barrier that opens up when approached autoamtically.
		* It blocks a node until it is approached and then unlocks that node until no new appraoches have been registered for a defined amount of time.
		*/
		class QSF_AI_API_EXPORT SimpleBarrierComponent : public DynamicNavigationBlockerComponent
		{
		public:
			// Unique CAMP Ids for the component, the exported properties and default values for the properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const ExportedProperty<Time> TIME_TO_OPERATE_BARRIER; // time it takes the barrier to go up or down
			static const ExportedProperty<Time> DELAY_AFTER_PASSAGE; // time to keep the barrier open after the entity has passed
			//@}

			// simple state for the barrier
			enum State
			{
				DOWN,
				GOING_UP,
				UP,
				GOING_DOWN,
			};

			SimpleBarrierComponent(Prototype* prototype);

			// overridden component interface
			//@{
			virtual bool onStartup() override;
			virtual bool implementsOnComponentPropertyChange() const override;
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// overridden DynamicNavigationBlockerComponent
			virtual effort::Indicator update(const Time& now) override;

			Time getOperateBarrierTime() const;
			void setOperateBarrierTime(Time time);

			Time getCloseDelayAfterPassage() const;
			void setCloseDelayAfterPassage(Time delay);

		private:
			State mState; // current barrier state
			Time mCurrentStateIsFinishedAt; // time when the next state change is planned

			// TODO(vs) These values are currently more dummies
			Time mOperateBarrierTime; // time it takes to open or close the barrier
			Time mKeepOpenAfterEntityPassedTime; // the additional time it takes to keep the barrier open after the last requesting entity has passed

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline SimpleBarrierComponent::SimpleBarrierComponent(Prototype* prototype) :
		DynamicNavigationBlockerComponent(prototype),
		mState(DOWN),
		mCurrentStateIsFinishedAt(Time::MAX),
		mOperateBarrierTime(TIME_TO_OPERATE_BARRIER.mDefault),
		mKeepOpenAfterEntityPassedTime(DELAY_AFTER_PASSAGE.mDefault)
		{}

		inline Time SimpleBarrierComponent::getOperateBarrierTime() const
		{
			return mOperateBarrierTime;
		}

		inline void SimpleBarrierComponent::setOperateBarrierTime(Time time)
		{
			assignAndPromoteChange(mOperateBarrierTime, time, TIME_TO_OPERATE_BARRIER.mId);
		}

		inline Time SimpleBarrierComponent::getCloseDelayAfterPassage() const
		{
			return mKeepOpenAfterEntityPassedTime;
		}

		inline void SimpleBarrierComponent::setCloseDelayAfterPassage(Time time)
		{
			assignAndPromoteChange(mKeepOpenAfterEntityPassedTime, time, DELAY_AFTER_PASSAGE.mId);
		}

		inline bool SimpleBarrierComponent::implementsOnComponentPropertyChange() const
		{
			return true;
		}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SimpleBarrierComponent);
