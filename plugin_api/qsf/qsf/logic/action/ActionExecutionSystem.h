// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/LogicSystem.h"
#include "qsf/logic/EffortIndicator.h"
#include "qsf/logic/action/ScriptActionManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class ActionComponent;
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
	*    System to execute actions from a plan
	*
	*  @note
	*  - Entities that have an action component attached are registered here
	*  - Noncopyable by inheritance
	*/
	class QSF_API_EXPORT ActionExecutionSystem : public LogicSystem
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ActionExecutionSystem();

		/**
		*  @brief
		*    Return the instance of the script action manager, don't destroy the instance
		*/
		inline ScriptActionManager& getScriptActionManager();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::LogicSystem methods            ]
	//[-------------------------------------------------------]
	protected:
		virtual uint32 getJobManagerId() override;
		virtual void updateLogicSystem(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		effort::Indicator updateComponent(ActionComponent& component, const Clock& clock);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ScriptActionManager	mScriptActionManager;	///< Instance of script action manager, we're responsible for destroying the instance in case we no longer need them


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/action/ActionExecutionSystem-inl.h"
