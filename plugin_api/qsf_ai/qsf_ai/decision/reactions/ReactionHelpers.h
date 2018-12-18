// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionRule.h"

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Action;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/** Class a concrete rule may want to use
		* in case it needs to define some kind of delay in seconds.
		* For example a waiting reaction on a rain-starts-event
		* could use it to avoid that all civilians opening their umbrella at the very same time.
		*
		* Additional JSON configuration tags:
		* MinDelay, MaxDelay
		*/
		class QSF_AI_API_EXPORT MinMaxDelay
		{
			// Tag string IDs used by the JSON serializer.
			//@{
			static const char* MIN_DELAY_TAG;
			static const char* MAX_DELAY_TAG;
			//@}
		public:
			MinMaxDelay();
			void deserialize(const boost::property_tree::ptree& tree);
			void serialize(boost::property_tree::ptree& tree) const;
			bool isUsed() const;
			float getRandomDelayInSeconds() const;
		private:
			UnsignedFloat mMinDelay;
			UnsignedFloat mMaxDelay;
		};

		/** Class a concrete rule may want to use
		* in case it needs to define some kind of min/max duration in seconds.
		* For example a waiting rule may use it to define the
		* waiting period.
		*
		* Additional JSON configuration tags:
		* MinDuration, MaxDuration
		*/
		class QSF_AI_API_EXPORT MinMaxDuration
		{
			// Tag string IDs used by the JSON serializer.
			//@{
			static const char* MIN_DURATION_TAG;
			static const char* MAX_DURATION_TAG;
			//@}
		public:
			MinMaxDuration();
			MinMaxDuration(UnsignedFloat minDuration,UnsignedFloat maxDuration);
			void init(UnsignedFloat minDuration,UnsignedFloat maxDuration);
			void deserialize(const boost::property_tree::ptree& tree);
			void serialize(boost::property_tree::ptree& tree) const;
			bool isUsed() const;
			float getRandomDurationInSeconds() const;
		private:
			UnsignedFloat mMinDuration;
			UnsignedFloat mMaxDuration;
		};


		/** Class a concrete rule may want to use
		* in case it needs to turn to the perceiver.
		*
		* Additional JSON configuration tags:
		* TurnReverse
		*/
		class QSF_AI_API_EXPORT TurnToPerceiver
		{
			// Tag string IDs used by the JSON serializer.
			//@{
			static const char* TURN_REVERSE_TAG;
			//@}
		public:
			TurnToPerceiver();
			void deserialize(const boost::property_tree::ptree& tree);
			void serialize(boost::property_tree::ptree& tree) const;
			bool getTurnReverse() const;
		private:
			bool mTurnReverse;
		};


		/** Class a concrete rule may want to use
		* in case it needs to play an animation.
		*
		* Additional JSON configuration tags:
		* AnimationPath
		*/
		class QSF_AI_API_EXPORT Animation
		{
			// Tag string IDs used by the JSON serializer.
			//@{
			static const char* ANIMATION_TAG;
			//@}

		public:
			void deserialize(const boost::property_tree::ptree& tree);
			void serialize(boost::property_tree::ptree& tree) const;
			const std::string& getAnimationPath() const;

		private:
			std::string mAnimationPath;
		};


		/** Simplifies the creation of action plans as often required
		* from inside a reaction-rule's handle-method.
		*/
		class QSF_AI_API_EXPORT PlanActions
		{
		public:
			PlanActions(const EvaluationParameters& parameters, const PerceptionEvent* evt);
			PlanActions(const EvaluationParameters& parameters, const EvaluationParameters::PerceptionEventIterator& eventIterator);
			PlanActions(const Entity& entity, const PerceptionEvent* evt, action::Priority priority);
			~PlanActions();
			operator bool() const;

			action::Priority getPriority() const;
			void setPriority(action::Priority priority);

			bool isCurrentlyEmpty() const;
			action::Priority getCurrentActionsPriority() const;

			template<typename ActionType> ActionType& addAction();
			void clear();
			void delay(float seconds);
			void delay(const MinMaxDelay& delay);
			void waitFor(const MinMaxDuration& duration);
			void turnTo(uint64 entityId);
			void turnTo(const TurnToPerceiver& turnto);
			void play(const std::string& animPath,bool loop=false,bool reverse=false);
			void play(const Animation& animation);

		private:
			ActionComponent* mActionComponent;
			const PerceptionEvent* mEvent;
			action::Priority mPriority;
			bool mClearPlan;
			std::vector<Action*> mActions;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionHelpers-inl.h"
