// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/logic/action/ActionPlan.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class PropertyDictionary;
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
	*    Action component
	*
	*  @remarks
	*    A component for entities that may execute actions according to their scheduled plan
	*
	*  @note
	*    - Noncopyable like all components by inheritance
	*/
	class QSF_API_EXPORT ActionComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Unique CAMP IDs for the component and the plan property
		//@{
		static const uint32 COMPONENT_ID;
		static const uint32 PLAN_PROPERTY_ID;
		//@}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline ActionComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ActionComponent();

		/**
		*  @brief
		*    Return the action plan
		*/
		inline const ActionPlan& getPlan() const;

		/**
		*  @brief
		*    Clear the action plan
		*/
		void clearPlan();

		/**
		*  @brief
		*    Insert a new action into the action plan
		*
		*  @param[in] newAction
		*    The action to insert, ownership is transferred to the action plan, so do not destroy the instance yourself later
		*  @param[in] priority
		*    The priority to give to the inserted action
		*  @param[in] insertionMode
		*    Defines how and where the action is inserted into the plan
		*/
		void insertAction(Action& newAction, action::Priority priority, action::InsertionMode insertionMode);

		/**
		*  @brief
		*    Create and insert an action into the action plan
		*
		*  @param[in] actionTypeId
		*    The type ID of the action to insert
		*  @param[in] parameters
		*    Parameters as a PropertyDictionary
		*  @param[in] priority
		*    The priority to give to the inserted action
		*  @param[in] insertionMode
		*    Defines how and where the action is inserted into the plan
		*
		*  @return
		*    A instance of the class
		*/
		Action* pushActionByIdWithParameters(const StringHash& actionTypeId, const PropertyDictionary* parameters, action::Priority priority = 100, action::InsertionMode insertionMode = action::APPEND_TO_BACK);

		/**
		*  @brief
		*    Create and insert an action into the action plan
		*
		*  @param[in] actionTypeId
		*    The type ID of the action to insert
		*  @param[in] priority
		*    The priority to give to the inserted action
		*  @param[in] insertionMode
		*    Defines how and where the action is inserted into the plan
		*
		*  @return
		*    A instance of the class
		*/
		inline Action* pushActionById(const StringHash& actionTypeId, action::Priority priority = 100, action::InsertionMode insertionMode = action::APPEND_TO_BACK);

		/**
		*  @brief
		*    Create and insert an action into the action plan
		*
		*  @param[in] actionName
		*    The string name of the type ID of the action to insert
		*  @param[in] priority
		*    The priority to give to the inserted action
		*  @param[in] insertionMode
		*    Defines how and where the action is inserted into the plan
		*  @return
		*    A instance of the class
		*
		*  @remarks
		*    This function is used via script interface
		*/
		Action* pushActionByName(const std::string& actionName, action::Priority priority = 100, action::InsertionMode insertionMode = action::APPEND_TO_BACK);

		/**
		*  @brief
		*    Create and insert an action into the action plan
		*
		*  @param[in] actionName
		*    The string name of the type ID of the action to insert
		*  @param[in] priority
		*    The priority to give to the inserted action
		*  @param[in] insertionMode
		*    Defines how and where the action is inserted into the plan
		*
		*  @return
		*    A instance of the class
		*
		*  @remarks
		*    This function is the default for pushing actions from cpp. Access the given class directly for calling eg init().
		*    Usage example:
		*    @code
		*    pushAction<CutTreeAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		*    @endcode
		*/
		template<typename T>
		T& pushAction(action::Priority priority = 100, action::InsertionMode insertionMode = action::APPEND_TO_BACK);

		/**
		*  @brief
		*    Return the current action or nullptr
		*/
		inline Action* getCurrentAction() const;

		/**
		*  @brief
		*    Return a action of the given type
		*
		*  @remarks
		*    Internally calls getActionByTypeId() with the action type of the template parameter
		*    @code
		*    EnterVehicleAction* enterVehicleAction = actionComponent.getAction<EnterVehicleAction>();
		*    @endcode
		*/
		template<typename T>
		T* getAction() const;

		/**
		*  @brief
		*    Return an action of the given type ID
		*/
		const Action* getActionByTypeId(uint32 aictonId) const;

		/**
		*  @brief
		*    Removes the given action
		*
		*  @param[in] action
		*    Action instance to remove from this component
		*
		*  @return
		*    "true" in case the action was actually found and removed, "false" otherwise
		*/
		inline bool removeAction(Action& action);

		/**
		*  @brief
		*    Return priority of current action
		*
		*  @return
		*    Priority of the current action, or 0 in case the action plan is empty
		*/
		inline action::Priority getCurrentPriority() const;

		/**
		*  @brief
		*    Update the current action
		*
		*  @param[in] clock
		*    The clock used for timing the action updates; holds e.g. time passed since last update
		*/
		void updateCurrentAction(const Clock& clock);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool onStartup() override;
		inline virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static Action* createActionById(const StringHash& actionTypeId, const PropertyDictionary* parameters = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ActionPlan mPlan;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ActionComponent);


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/action/ActionComponent-inl.h"
