// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/decision/reactions/ReactionRulesTable.h"
#include "qsf_ai/base/UpdateInterval.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/component/Component.h>
#include <qsf/logic/EffortIndicator.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	class ActionComponent;

	namespace ai
	{
		class PerceptionEvent;
		class ReactionSystem;

		/** Allows entities to come to decision by reacting on certain input.
		*/
		class QSF_AI_API_EXPORT ReactionComponent : public Component
		{
		public:
			// unique CAMP ids for the component and for properties and default values
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int REACTION_RULES_ASSET_PROPERTY_ID;
			static const unsigned int UPDATE_INTERVAL_PROPERTY_ID;
			static const unsigned int UPDATE_INTERVAL_JITTER_PROPERTY_ID;
			static const unsigned int MIN_PRIORITY_PROPERTY_ID;

			static const std::string DEFAULT_REACTION_RULES_ASSET;
			static const UnsignedFloat DEFAULT_UPDATE_INTERVAL;
			static const UnsignedFloat DEFAULT_UPDATE_INTERVAL_JITTER;
			static const unsigned int DEFAULT_MIN_PRIORITY;
			//@}

			ReactionComponent(Prototype* prototype);

			// Overridden Component interface
			//@{
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Process the component's ReactionRulesTable.
			effort::Indicator update(const JobArguments* jobArguments);

			// Add a PerceptionEvent to the ReactionComponent's event-queue
			// if this component is active. If it is not then the event is ignored.
			// This is the preferred way for OTHER (!) entities to notify this entity.
			// This entity's OWN sensors should usually NOT push events through this interface
			// (by using the sensor's AutoPushEvents feature)
			// because it is more efficient if this entity "polls" its sensors when
			// it actually needs sensor-information (= when a rule is actually evaluated).
			void pushEvent(const PerceptionEvent& perceptionEvent);

			// Access the component's minimal reaction-rule-priority.
			// For AI LOD. Only rules with a priority higher or equal to it are even considered for execution.
			// Defaults to 0, meaning every rule is considered.
			//@{
			action::Priority getMinPriority() const;
			void setMinPriority(action::Priority minPriority);
			//@}

			// The automatic update interval in seconds.
			// DANGER: this defaults to 0 for a reason:
			// With values >0 there's the chance that global events are "overseen".
			// Know what you're doing when modifying the update interval!
			// It is a great time-safer for use with things like waiting-points
			// but keep in mind the potential side-effects!
			//@{
			UnsignedFloat getUpdateInterval() const;
			void setUpdateInterval(UnsignedFloat updateInterval);
			//@}

			// Defines an optional random jitter value that is added to the
			// update-trigger-timer when resetting it to the update-interval.
			//@{
			UnsignedFloat getUpdateIntervalJitter() const;
			void setUpdateIntervalJitter(UnsignedFloat updateInterval);
			//@}

			// Get or set the asset that contains the ReactionComponent's
			// reaction-rules-table-configuration.
			// The internal mReactions-table is rebuilt if necessary.
			//@{
			const AssetProxy& getReactionRulesAsset() const;
			void setReactionRulesAsset(const AssetProxy& asset);
			//@}

			// Access to the reaction table
			const ReactionRulesTable& getReactions() const;

			// Access the local event queue
			PerceptionEventQueue& getPerceptionEventQueue();

		protected:
			// Overridden Component interface
			//@{
			virtual bool onStartup();
			//@}
		private:
			action::Priority mMinPriority; // minimal priority for AI LOD. Only rules with a priority higher or equal to it are even considered for execution.
			const ReactionSystem* mReactionSystem; // pointer to the ReactionSystem (cached in onStartup).
			ReactionRulesTable mReactions; // the actual list of prioritized reaction/condition rules.
			UpdateInterval mUpdateInterval;  // manages remaining time until the next automatic update, the interval and an optional jitter
			PerceptionEventQueue mPerceptionEventQueue; // the ReactionComponent's local event queue.
			AssetProxy mReactionRulesAsset; // Asset containing the component's reaction-rule-table configuration.
			WeakPtr<ActionComponent> mActionComponent;	// Direct component references for efficient access
			QSF_CAMP_RTTI();
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::ReactionComponent);
