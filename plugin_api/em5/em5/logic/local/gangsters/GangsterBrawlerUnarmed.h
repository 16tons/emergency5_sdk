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
	*    Game logic for gangster type "brawler unarmed"
	*/
	class EM5_API_EXPORT GangsterBrawlerUnarmed : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterBrawlerUnarmed" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterBrawlerUnarmed();
		void setAggroRange(float range);
		void setDeAggroRange(float val);
		void setHitCoolDownTime(qsf::Time val);
		bool hasTarget() const;
		void setTarget(uint64 targetId);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;
		// Note: Don't simply save the list because there is no possibility to make sure that the entity references stay valid after the method returns
		void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) override;

		// Notifies us when no more police units are within range. We use that to change the gangster back from "killer taskforce" to his last gangster logic
		void onNoUnitsSpotted() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* getNextTarget() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64	mCurrentTargetId;	///< Current target this gangster is going to attack
		float mAggroRange;
		float mDeAggroRange;

		qsf::Time mHitCoolDown;
		qsf::Time mHitCoolDownTime;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterBrawlerUnarmed)
