// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"


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
	*    Game logic for gangster type used in "City blackout" campaign mini-event
	*/
	class EM5_API_EXPORT GangsterBlackoutMini : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterBlackoutMini" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterBlackoutMini();

		void setCar(qsf::Entity& entity);
		void setTargetPointForDriving(qsf::Entity& entity);
		void setBombPoint(qsf::Entity& entity);
		void setTriggerPoint(qsf::Entity& entity);
		void setTimeToDefuseBomb(qsf::Time time);
		void setTimeToWaitForExplosion(qsf::Time time);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;
		void onIdle() override;
		void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) override;
		void onNoUnitsSpotted() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_ENTER_CAR,
			STATE_DRIVE_TO_TARGET,
			STATE_GO_TO_BOMB_POINT,
			STATE_LAY_BOMB,
			STATE_GO_TO_TRIGGER_POINT,
			STATE_WAIT_AT_TRIGGER_POINT,
			STATE_ESCAPE
		};
		State mState;

		qsf::WeakPtr<qsf::Entity> mCar;
		qsf::WeakPtr<qsf::Entity> mTargetPointForDriving;
		qsf::WeakPtr<qsf::Entity> mBombPoint;
		qsf::WeakPtr<qsf::Entity> mTriggerPoint;
		qsf::Time mTimeToDefuseBomb;
		qsf::Time mTimeToWaitForExplosion;

		qsf::WeakPtr<qsf::Entity> mBombEntity;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterBlackoutMini)
