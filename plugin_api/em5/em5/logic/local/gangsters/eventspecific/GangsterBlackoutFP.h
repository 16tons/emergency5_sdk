// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"

#include <qsf/base/NamedIdentifier.h>

#include <deque>


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
	*    Game logic for gangster type used in "City blackout" freeplay event
	*/
	class EM5_API_EXPORT GangsterBlackoutFP : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterBlackoutFP" game logic type ID
		static const qsf::NamedIdentifier MESSAGE_ON_EBOX_DAMAGE;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterBlackoutFP();

		void setTargetEboxes(const std::vector<qsf::Entity*>& eboxes);
		void setTimeToDamageEbox(qsf::Time time);


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
		std::deque<qsf::WeakPtr<qsf::Entity>> mTargetEboxes;		///< Contains only the remaining eboxes
		qsf::Time mTimeToDamageEbox;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterBlackoutFP)
