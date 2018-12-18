// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/PerceptionEvent.h"

#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/** PerceptionEventAction is an action that fires a perception-event
		* if started up. It can also be configured to even fire the desired
		* event if it gets aborted.
		*/
		class QSF_AI_API_EXPORT PerceptionEventAction : public Action
		{
		public:
			static const NamedIdentifier ACTION_ID;	///< "qsf::ai::PerceptionEventAction" unique action identifier

		public:
			PerceptionEventAction();

			// initialize this action by defining the concrete PerceptionEvent to fire.
			// If pushAsGlobalEvent is true the event is put into the global event queue, if it is false then
			// it is pushed into the local event queue of the entity that's specified as mEntity in the event itself.
			// If fireEventEvenIfNotExecuted is true, then the event is also fired, if the action is aborted before it
			// is started up.
			void init(const PerceptionEvent& perceptionEvent, bool pushAsGlobalEvent, bool fireEventEvenIfNotExecuted);

		public:
			virtual void serialize(BinarySerializer& serializer) override;

		protected:
			virtual bool onStartup();
			virtual void onAbort();
			virtual action::Result updateAction(const Clock& clock) override;

		private:
			// the actual event firing. Called internally by onStartup or onAbort.
			void fireEvent();

		private:
			bool mPushAsGlobalEvent;
			bool mFireEventEvenIfNotExecuted;
			PerceptionEvent mPerceptionEvent;

			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::PerceptionEventAction)
