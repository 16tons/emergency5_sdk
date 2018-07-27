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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingComponent;
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
	*    Game logic for gangster type "Arsonist"
	*/
	class EM5_API_EXPORT GangsterArsonist : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterArsonist" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterArsonist();
		void setCheeringTime(qsf::Time val) { mCheeringTime = val; }


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;

		// Note: Don't simply save the list because there is no possibility to make sure that the entity references stay valid after the method returns
		virtual void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) override;

		// Notifies us when no more police units are within range. We use that to change the gangster back from "killer taskforce" to his last gangster logic
		virtual void onNoUnitsSpotted() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		BuildingComponent* findBuildingCandidate() const;
		bool checkBuildingCandidate(qsf::Entity& building) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64 mCurrentTargetId;
		qsf::Time mCheeringTime;

		enum NextAction
		{
			UNDEFINED = 0,
			GO_TO_NEXT_BUILDING,		///< Go to closest building
			SET_BUILDING_ON_FIRE,		///< Set that building on fire
			WATCH_AND_CHEER,			///< Step away from the building and cheer
			DO_NOTHING
		};
		NextAction mNextAction;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterArsonist)
