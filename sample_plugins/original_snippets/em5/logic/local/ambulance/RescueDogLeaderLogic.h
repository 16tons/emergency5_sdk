// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"
#include "em5/logic/local/base/CarryPersonLogic.h"

#include <qsf/map/Entity.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ActionComponent;
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
	*    Rescue dog leader logic
	*/
	class EM5_API_EXPORT RescueDogLeaderLogic : public CarryPersonLogic
	{


	//[-------------------------------------------------------]
	//[ Static public methods                                 ]
	//[-------------------------------------------------------]
	public:
		static qsf::Entity* createRescueDogEntity(qsf::Entity& rescueDogLeaderEntity);
		static qsf::Entity* getRescueDogEntityFromLeader(qsf::Entity& rescueDogLeaderEntity);
		static void delinkDogFromLeader(qsf::Entity& rescueDogLeaderEntity);
		static void linkDogWithLeader(qsf::Entity& rescueDogLeaderEntity, bool leavingVehicle = false);
		static void callRescueDogBack(qsf::Entity& rescueDogLeaderEntity);
		static void playDogPantAudio(qsf::Entity& rescueDogLeaderEntity, bool pant);
		// An alternate RescueDogLeaderLogic::getIsDogUnitedWithLeader() implementation that works on multiplayer clients but is more complex
		static bool checkIsRescueDogUnitedWithLeader(qsf::Entity& rescueDogLeaderEntity);


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::RescueDogLeaderLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		RescueDogLeaderLogic();

		void setIsDogUnitedWithLeader(bool value);
		bool getIsDogUnitedWithLeader();

		void setRescueDog(qsf::Entity* dogEntity);
		qsf::Entity* getRescueDog();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;	///< Action priority


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    React to the exit vehicle command
		*/
		void takeDogFromVehicle(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    React to the enter vehicle command
		*/
		void putDogInVehicle(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    React to the on person injured message
		*/
		void dieRescueDog(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    React when leader got contaminated
		*/
		void onContaminated(const qsf::MessageParameters& parameters);

		void updateStuckResolving(const qsf::JobArguments& jobArguments);

		void startWaitForEnterVehicle(const qsf::MessageParameters& parameters);
		void abortWaitForEnterVehicle(const qsf::MessageParameters& parameters);

		void pushSignalActionAndSetupMessageHandler(uint64 vehicleEntityId, qsf::ActionComponent& actionComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time					mTimeWaiting;
		qsf::WeakPtr<qsf::Entity>	mRescueDogEntity;
		bool						mIsDogUnitedWithLeader;	///< "true" if the dog and the leader are one unit, "false" if the dog is moving without the leader
		AudioProxy					mAudioProxy;
		qsf::Time					mSafetyStuckResolvingTime;

		// We need this message proxy to get or put away the Dog and have a safe serialization
		qsf::MessageProxy mExitVehicleProxy;			///< Message proxy for post command messages
		qsf::MessageProxy mEnterVehicleProxy;			///< Message proxy for on command messages
		qsf::MessageProxy mLeaderInjured;				///< Message proxy for on command messages
		qsf::MessageProxy mLeaderContaminated;			///< Message proxy for on command messages
		qsf::JobProxy	  mSafetyStuckResolvingJob;		///< Job proxy for detecting stucking after getting the rescuedog
		qsf::MessageProxy mEnterVehicleWithoutEquipmentProxy;
		qsf::MessageProxy mEnterVehicleWithoutEquipmentAbortProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RescueDogLeaderLogic)
