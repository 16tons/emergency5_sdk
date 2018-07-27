// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

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
	*    EMERGENCY 5 drone transport logic
	*/
	class EM5_API_EXPORT DroneTransportLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::DroneTransportLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DroneTransportLogic();

		qsf::Entity* getDroneEntity() const;

		bool getIsDroneUnitedWithLeader() const;
		void setIsDroneUnitedWithLeader(bool value);

		/**
		*  @brief
		*    Returns the position where the drone pod is located on the transporter
		*/
		const glm::vec3& getLocalDronePodPosition() const;


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
		void onTransporterStartsBurning(const qsf::MessageParameters& parameters);
		void onChangeSelectionSEKDroneTransporter(const qsf::MessageParameters& parameters);
		glm::vec3 calcClosestPointToBorders(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity>	mDroneEntity;
		bool						mIsDroneUnitedWithTransporter;
		glm::vec3					mLocalDronePodPosition;	// The local drone pod (where the drone is located when landed) position relative to transporter pivot

		// Messages
		qsf::MessageProxy	mOnStartBurningProxy;			///< Message proxy for receiving the "EM5_FIRECOMPONENT_START_BURNING" message for the parent
		qsf::MessageProxy	mOnSelectionChangeProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DroneTransportLogic)
