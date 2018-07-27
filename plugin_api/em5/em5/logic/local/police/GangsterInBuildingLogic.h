// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/building/BuildingComponent.h"

#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/base/PropertyHelper.h>


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
	*    Gangster in building logic
	*/
	class EM5_API_EXPORT GangsterInBuildingLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterInBuildingLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterInBuildingLogic();

		// "true" if the right number of police units is inside the building
		bool isPoliceTeamComplete() const;

		void onPoliceSpokeToHostageTaker();
		void onPoliceAttackedBuilding();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AllowNormalPolice, bool, mAllowNormalPolice)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CanSpeakWithHostageTaker, bool, mCanSpeakWithHostageTaker)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FakeObject, bool, mFakeObject)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(InUseFromSpeaker, bool, mInUseFromSpeaker)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterGiveUp, bool, mGangsterTakeGiveUp)


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
		static const uint32 ACTION_PRIORITY;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<BuildingComponent> mBuildingComponent;

		bool mAllowNormalPolice;
		bool mCanSpeakWithHostageTaker;
		bool mFakeObject;				// If "true", no real gangster are inside
		bool mInUseFromSpeaker;
		bool mGangsterTakeGiveUp;		// By default random value, depends on value in LogicSpecsGroup

		// Internal
		bool mCanSetHostageObjectives;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterInBuildingLogic)
