// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/logic/action/Action.h>
#include <qsf/debug/DebugDrawProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{

	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Emergency 5 set building on fire action
	*/
	class SetBuildingOnFireAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::SetBuildingOnFireAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SetBuildingOnFireAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SetBuildingOnFireAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& target, FreeplayEvent &relatedEvent);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64						mTargetEntityId; ///< Target entity of the action
		FreeplayEvent*				mRelatedEvent;   ///< Event that is related to this action and needed for setting buildings on fire
		qsf::DebugDrawProxy			mDebugDrawProxy; ///< Debug draw proxy for text output

	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SetBuildingOnFireAction)
