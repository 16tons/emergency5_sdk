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
	*    Game logic for gangster type "killer psycho"
	*
	*/
	class EM5_API_EXPORT GangsterKillerPsychoLogic : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterKillerPsychoLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterKillerPsychoLogic();


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	protected:
		void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;
		// Note: Don't simply save the list because there is no possibility to make sure that the entity references stay valid after the method returns
		void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64	mCurrentTargetId; ///< The id of the entity the gangster is going to attack next / currently attacking


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterKillerPsychoLogic)
