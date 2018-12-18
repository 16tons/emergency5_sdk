// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>
#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}


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
	*    Pull object logic
	*
	*  @remarks
	*    This game logic is located in an entity that pulls another entity.
	*/
	class EM5_API_EXPORT PullObjectLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::PullObjectLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PullObjectLogic();

		/**
		*  @brief
		*    Check if the person is pulling an object
		*
		*  @return
		*    "true" when the person is pulling an object, "false" otherwise
		*/
		bool isPullingObject() const;

		/**
		*  @brief
		*    Returns pulled object
		*
		*  @return
		*    The pulled entity
		*/
		qsf::Entity* getPulledObject() const;

		/**
		*  @brief
		*    Set pulled object
		*
		*  @param[in] entity
		*    The pulling entity, may be a null pointer
		*/
		void setPulledObject(qsf::Entity* entity);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateSimulation(const qsf::JobArguments&);
		void onPersonInjured(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy				mSimulationJobProxy;
		qsf::WeakPtr<qsf::Entity>	mPulledEntity;			///< The pulled entity
		qsf::MessageProxy			mMessageProxyInjured;	///< Registered at person injured message


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PullObjectLogic)
