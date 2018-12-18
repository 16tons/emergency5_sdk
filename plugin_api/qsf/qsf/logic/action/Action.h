// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/action/ActionPriority.h"
#include "qsf/logic/action/ActionResult.h"
#include "qsf/logic/ProcessState.h"
#include "qsf/time/clock/Clock.h"
#include "qsf/base/NamedIdentifier.h"
#include "qsf/map/Entity.h"			// Required, else CAMP will not be able to create instances because the CAMP type "qsf::Entity" will be unknown


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ActionComponent;
	class Map;
	class BinarySerializer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract action base class
	*
	*  @remarks
	*    An action is an atomic piece of behavior.
	*    Entities may execute actions when they have an action component attached.
	*    An action defines the rules of the described behavior within its update function.
	*    Actions have a priority value that defines which other behavior may currently interrupt.
	*    The simple rule is that behavior with a higher priority is more important to execute and it may therefore interrupt all running actions with a lower priority.
	*    Fleeing to save your life will therefore have a high priority for example while idling around will have a minimal priority.
	*    Actions are non-copyable.
	*
	*  @note
	*    - TODO(vs) There is currently no automatism to ensure correct usage of priorities and it is the duty of every system that pushes actions onto a plan to pay attention to the priority.
	*      This could be tweaked.
	*/
	class QSF_API_EXPORT Action : public Object
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ActionComponent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Action();

		/**
		*  @brief
		*    Return the action's type ID
		*/
		inline uint32 getTypeId() const;

		/**
		*  @brief
		*    Return the owning action component
		*/
		inline ActionComponent& getComponent() const;

		/**
		*  @brief
		*    Return the owning entity
		*/
		Entity& getEntity() const;

		/**
		*  @brief
		*    Return the owning entity's ID
		*/
		uint64 getEntityId() const;

		/**
		*  @brief
		*    Return the map the action's owning entity is in
		*/
		Map& getMap() const;

		/**
		*  @brief
		*    Return the action's process state
		*/
		inline process::State getState() const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Update the action
		*
		*  @param clock
		*    The clock used for timing the action updates; holds e.g. time passed since last update
		*  @return
		*    The result of the action update
		*/
		action::Result update(const Clock& clock);

		/**
		*  @brief
		*    Called before the action gets removed
		*/
		void shutdown();

		/**
		*  @brief
		*    Called when the action gets interrupted by another action, but not removed
		*/
		void pause();

		//[-------------------------------------------------------]
		//[ Priority access                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get action priority
		*/
		action::Priority getPriority() const;

		/**
		*  @brief
		*    Set action priority
		*/
		void setPriority(action::Priority priority);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Serialize or deserialize the action using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		virtual void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		inline Action(uint32 actionTypeId);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Update the action
		*
		*  @param clock
		*    The clock used for timing the action updates; holds e.g. time passed since last update
		*
		*  @return
		*    The result of the action update
		*/
		virtual action::Result updateAction(const Clock& clock) = 0;

		/**
		*  @brief
		*    Called when the action gets active (i.e. switches to the "running" state) for the first time
		*
		*  @return
		*    true if startup succeeded, false otherwise
		*/
		inline virtual bool onStartup();

		/**
		*  @brief
		*    Called before the action gets removed, in case it was active at least once since its creation
		*
		*  @remarks
		*    This method is called only if the action to be removed got an onStartup() call before;
		*    if this was not the case, onAbort() gets called instead
		*/
		inline virtual void onShutdown();

		/**
		*  @brief
		*    Called before the action gets removed, in case it was never active
		*/
		inline virtual void onAbort();

		/**
		*  @brief
		*    Called when the action gets interrupted by another action, but not removed
		*/
		inline virtual void onPause();

		/**
		*  @brief
		*    Called when the action gets active again after it was paused
		*/
		inline virtual void onResume();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// noncopyable
		inline Action(const Action&);
		inline Action& operator=(const Action&);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		action::Priority	mPriority;			///< Action with higher priority get evaluated first; default value is 100
		bool				mIsScriptAction;	///< True if action is loaded via script


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			 mActionTypeId;		///< Type ID of this action; this is the concrete action class's hashed name
		ActionComponent* mOwnerComponent;	///< Owning action component; set by the action component
		process::State	 mState;			///< Current state of the action


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


// Temporary dummy macro to be added to all actions that don't have a real serialization function yet
// TODO(vs) Remove this once we have real serialization methods for all actions
#define QSF_ADD_EMPTY_DUMMY_ACTION_SERIALIZATION inline virtual void serialize(qsf::BinarySerializer& serializer) override {};


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/action/Action-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Action)
