// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    Game logic for gangster type "Killer taskforce". This bundles the "move" and the "fixed" types.
	*
	*  @note
	*	 - The actual move/fixed logic is determined from the weapons the gangster equips
	*/
	class EM5_API_EXPORT GangsterKillerTaskforceLogic : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterKillerTaskforceLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterKillerTaskforceLogic();


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	protected:
		void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;
		// Note: Don't simply save the list because there is no possibility to make sure that the entity references stay valid after the method returns
		void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) override;

		// Notifies us when no more police units are within range. We use that to change the gangster back from "killer taskforce" to his last gangster logic
		void onNoUnitsSpotted() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64	mCurrentTargetId;	///< Current target this gangster is going to attack
		bool	mPursueTargets;		///< This effectively enables the "killer taskforce, move" logic by forcing the pursuit of targets when necessary. It is set automatically when no firearms are equipped.


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterKillerTaskforceLogic)
