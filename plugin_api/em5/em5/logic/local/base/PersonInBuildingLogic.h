// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/message/MessageProxy.h>


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
	*    Person in building logic
	*/
	class EM5_API_EXPORT PersonInBuildingLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::PersonInBuildingLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PersonInBuildingLogic();

		void clearAllowedUnits();
		void addAllowedUnit(const qsf::StringHash& unitType);

		bool isTeamComplete() const;
		bool isUnitAllowed(const qsf::Entity& entity);

		void startFreeingCivil();
		bool isBuildingEmpty();

		void playEvacuateAudio();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(NumberValidTeamUnitsNeeded, int, mNumberValidTeamUnitsNeeded);


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
		void updateSimulation(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<BuildingComponent> mBuildingComponent;
		int	mNumberValidTeamUnitsNeeded;
		qsf::JobProxy		mUpdateJobProxy;

		bool mIsJobActive;
		qsf::Time	mRescuePersonTimer;
		qsf::Time	mRescuePersonTimerMax;

		AudioProxy		mAudioProxy;

		boost::container::flat_set<qsf::StringHash> mAllowedUnits;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonInBuildingLogic)
