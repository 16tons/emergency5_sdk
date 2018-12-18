// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/perception/PerceptionEventQueue.h"

#include <qsf/logic/action/ActionPriority.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
	class ActionComponent;
	namespace ai
	{
		class ReactionDebugGroup;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/** EvaluationParameters is a convenient wrapper to all those arguments
		* a condition usually needs to come to a conclusion.
		* It also contains a pointer to the entity's ActionComponent, if such a component exists.
		* Most important: it wraps and provides easy access to the different perception-event sources.
		*/
		class QSF_AI_API_EXPORT EvaluationParameters
		{
			friend class ReactionRulesTable; // to set mPriority
		public:

			/** Select the event source to be checked for events.
			* By selecting a specific source iteration over the events is speed up.
			*/
			enum PerceptionEventSource
			{
				DONT_CARE=0, // check all possible sources (not recommended)
				LOCAL, // check the local event queue only (usually the one that's attached to a ReactionComponent)
				GLOBAL, // check the global event queue only (used for global events like weather-events etc.)
				SENSOR, // only check the sensor(s) attached to the entity
				MAX_PERCEPTION_EVENT_SOURCE
			};

			// Helper methods in case the event-source shall be used with a JSON
			// and should appear in human-readable form.
			//@{
			static const std::string& PerceptionEventSourceToString(PerceptionEventSource source);
			static PerceptionEventSource PerceptionEventSourceFromString(const std::string& str);
			//@}


			/** PerceptionEventIterator is a simple iterator
			* to loop through all events of interest with ease.
			* It handles all the internals of addressing the different event-sources
			* so you can write a simple for-loop to check for events.
			*/
			class QSF_AI_API_EXPORT PerceptionEventIterator : public std::iterator<std::input_iterator_tag,PerceptionEvent*>
			{
			public:
				PerceptionEventIterator(const EvaluationParameters& evalutationParameters);
				PerceptionEventIterator(uint32 eventId,PerceptionEventSource eventSource,const EvaluationParameters& evalutationParameters);

				const PerceptionEvent& operator*() const;
				const PerceptionEvent* operator->() const;
				operator const PerceptionEvent*() const;
				bool operator==(const PerceptionEventIterator& rho) const;
				bool operator!=(const PerceptionEventIterator& rho) const;
				operator bool() const;
				PerceptionEventIterator& operator++();
				const PerceptionEvent* getPerceptionEvent() const;
			private:
				bool getNextEventQueue();
				PerceptionEventQueue* getNextEventQueueReal();
			private:
				PerceptionEventQueue::PerceptionEventIterator mQueueIterator;
				uint32 mEventId;
				uint32 mRealPerceptionEventSource;
				PerceptionEventSource mEventSource;
				PerceptionEventQueue* mCurrentEventQueue;
				const EvaluationParameters& mEvalutationParameters;
				PerceptionEventQueue mSensorEvents;
			};
		public:
			EvaluationParameters(Entity& entity,PerceptionEventQueue* localPerceptionEventQueue,PerceptionEventQueue* globalPerceptionEventQueue,ActionComponent* actionComponent,const JobArguments* jobArguments,ReactionDebugGroup* debugGroup);
			// Getters
			//@{
			Entity& getEntity() const;
			const JobArguments* getJobArguments() const;
			PerceptionEventQueue* getLocalPerceptionEventQueue() const;
			PerceptionEventQueue* getGlobalPerceptionEventQueue() const;
			ActionComponent* getActionComponent() const;
			ReactionDebugGroup* getDebugGroup() const;
			action::Priority getPriority() const;
			//@}

			// Methods to create the custom iterators to loop over the events.
			// Note: eventId has only been filled with a default value of 0 (= loop through all events) to
			// allow using simple c++ range loops (e.g. for (auto& i : evaluationParameters) ).
			// Whenever possible you should provide a concrete eventId and a concrete eventSource.
			//@{
			PerceptionEventIterator begin(uint32 eventId=0,PerceptionEventSource eventSource=DONT_CARE) const;
			PerceptionEventIterator end() const;
			//@}

			// This method allows a ReactionRule to manually force an attached sensor to update now.
			// This is especially useful if the sensor is not using its AutomaticUpdate feature.
			// By that the sensor's update can be executed exactly when it's needed - namely when
			// a ReactionRule is actually processed.
			// If eventId is 0 then all sensors are updated, otherwise only the sensor / sensor-configuration
			// that matches the eventId is updated.
			// An optional jobArgument parameter can be passed to only update if the sensor's update-timer
			// decides that it is actually time to update.
			// Returns true if this update may result in a perception-event.
			bool updateSensorNow(uint32 eventId,const JobArguments* jobArgument=nullptr) const;

			// Debug helper method to show a 1 second text-box near that entity.
			// For convenience, to give rule-authors a simple debug tool.
			void ShowDebugMessage(uint64 entityId,const std::string& msg) const;

		private:
			Entity& mEntity; // the entity whose rules are processed
			PerceptionEventQueue* mLocalPerceptionEventQueue; // the entity's ReactionComponent's local event queue
			PerceptionEventQueue* mGlobalPerceptionEventQueue; // the ReactionSystem's event queue
			ActionComponent* mActionComponent; // pointer to the entity's ActionComponent, if such a component exists (optional)
			const JobArguments* mJobArguments; // optional job-arguments
			ReactionDebugGroup* mDebugGroup; // optional debug-group
			action::Priority mPriority; // the priority at which the rule is defined in the reaction-table
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/EvaluationParameters-inl.h"
