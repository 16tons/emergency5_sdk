// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/reflection/ExportedProperty.h"
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/debug/group/DebugGroup.h>
#include <qsf/base/NamedIdentifier.h>
#include <qsf/base/UniqueInstance.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
	namespace ai
	{
		class ReactionSystem;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/**
		* Encapsulates the configurations for all available debugging options inside the AI reaction system.
		* Noncopyable by inheritance.
		*/
		class QSF_AI_API_EXPORT ReactionDebugGroup : public DebugGroup, public UniqueInstance<ReactionDebugGroup>
		{
		public:
			// Unique CAMP Ids and initial values
			//@{
			QSF_DECLARE_CAMP_PLUGINABLE_ID;
			static const unsigned int SHOW_FOR_ALL_ENTITIES_PROPERTY_ID; // Flag to force showing the debug output for all entities. Otherwise only the debug-output for the entity that's currently in debug state is generated.
			static const unsigned int SHOW_MESSAGES_PROPERTY_ID; // Flag to enable / disable text message debug output.
			static const unsigned int GLOBAL_TEST_EVENT_A_PROPERTY_ID; // Name/ID of a global event that will be fired every x seconds
			static const unsigned int GLOBAL_TEST_EVENT_B_PROPERTY_ID; // Name/ID of a global event that will be fired every x seconds
			static const unsigned int TEST_EVENT_A_INTERVAL_PROPERTY_ID; // interval in seconds at which TestEventA will be fired
			static const unsigned int TEST_EVENT_B_DELAY_PROPERTY_ID; // delay in seconds at which TestEventB will be fired after TestEventA was fired

			static const bool DEFAULT_SHOW_FOR_ALL_ENTITIES;
			static const bool DEFAULT_SHOW_MESSAGES;
			static const std::string DEFAULT_GLOBAL_TEST_EVENT;
			static const float DEFAULT_TEST_EVENT_A_INTERVAL;
			static const float DEFAULT_TEST_EVENT_B_DELAY;
			//@}

			// Constructor required by DebugGroup interface
			ReactionDebugGroup(DebugGroupManager* dgm);

			// Updates debug states that require timing, called internally by the ReactionSystem.
			// Handles firing of test-events.
			void update(ReactionSystem& reactionSystem,const JobArguments& jobArguments);

			// Getters and setters for the various debug options
			//@{
			bool getShowForAllEntities() const;
			void setShowForAllEntities(bool showForAllEntities);
			bool getShowMessages() const;
			void setShowMessages(bool showMessages);
			const std::string& getGlobalTestEventA() const;
			void setGlobalTestEventA(const std::string& globalTestEvent);
			const std::string& getGlobalTestEventB() const;
			void setGlobalTestEventB(const std::string& globalTestEvent);
			UnsignedFloat getFireTestEventAInterval() const;
			void setFireTestEventAInterval(UnsignedFloat interval);
			UnsignedFloat getFireTestEventBDelay() const;
			void setFireTestEventBDelay(UnsignedFloat interval);
			//@}

		private:
			bool mShowForAllEntities;
			bool mShowMessages;
			NamedIdentifier mGlobalTestEventA;
			NamedIdentifier mGlobalTestEventB;
			UnsignedFloat mFireTestEventAInterval;
			UnsignedFloat mFireTestEventBDelay;
			enum TestEventState
			{
				FIRE_A,
				FIRE_B,
			};
			TestEventState mTestEventState;
			float mTimeToTestEvent;
		private:
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
#include "qsf_ai/decision/reactions/ReactionDebugGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::ReactionDebugGroup);
