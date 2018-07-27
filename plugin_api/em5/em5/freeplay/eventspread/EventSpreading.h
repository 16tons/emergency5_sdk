// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/eventspread/EventSpreadReason.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}
namespace em5
{
	class FreeplayEvent;
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
	*    Static EMERGENCY 5 event spreading logic class
	*/
	class EventSpreading : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create generic objectives and observers at event spreading
		*/
		static bool eventSpreadReaction(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason = true);

		/**
		*  @brief
		*    Reaction to an object being set on fire
		*/
		static void startFireReaction(qsf::Entity& fireEntity);

		/**
		*  @brief
		*    Reaction to an object fire going out
		*/
		static void stopFireReaction(qsf::Entity& fireEntity, bool destroyed);

		static void enterContainerReaction(qsf::Entity& entity, qsf::Entity& containerEntity, uint32 containerCategory);
		static void leaveContainerReaction(qsf::Entity& entity, qsf::Entity& containerEntity, uint32 containerCategory);

		static void bombExplosionReaction(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		//[-------------------------------------------------------]
		//[ On event spread reason                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to event spread reason cause injury
		*/
		static bool onCauseInjury(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason cause injury
		*/
		static bool onCauseDead(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason spread fire
		*/
		static bool onSpreadFire(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason damage vehicle
		*/
		static bool onDamageVehicle(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason cause contamination
		*/
		static bool onSpreadContamination(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason new gangster
		*/
		static bool onNewGangster(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason specific
		*/
		static bool onEventSpecific(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason new gangster
		*/
		static bool onNewHostageInBuilding(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to event spread reason broken tree
		*/
		static bool onBrokenTree(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);


		//[-------------------------------------------------------]
		//[ On event reason cause injury                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to normal injury outside
		*/
		static bool onCauseInjury_Injury(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to normal injury in building
		*/
		static bool onCauseInjury_Injury_InBuilding(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to normal injury in road vehicle
		*/
		static bool onCauseInjury_Injury_InRoadVehicle(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to normal infection
		*/
		static bool onCauseInjury_Infection(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to infective infection
		*/
		static bool onCauseInjury_Infection_Infective(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to normal injury outside on gangsters
		*/
		static bool onCauseInjury_Gangster(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		//[-------------------------------------------------------]
		//[ On event reason cause dead                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to normal dead
		*/
		static bool onCauseDead_Dead(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		//[-------------------------------------------------------]
		//[ On event reason spread fire                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to entity is building
		*/
		static bool onSpreadFire_Building(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to entity is road vehicle
		*/
		static bool onSpreadFire_RoadVehicle(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to entity is object
		*/
		static bool onSpreadFire_Object(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		//[-------------------------------------------------------]
		//[ On event damage road vehicle                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to entity is road vehicle
		*/
		static bool onDamageVehicle_RoadVehicle(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		//[-------------------------------------------------------]
		//[ On event reason spread contamination                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to entity is person
		*/
		static bool onSpreadContamination_Person(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		//[-------------------------------------------------------]
		//[ On event reason new gangster                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to entity is gangster person
		*/
		static bool onNewGangster_Person(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to entity is gangster person and inside a building
		*/
		static bool onNewGangster_PersonInsideBuilding(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		//[-------------------------------------------------------]
		//[ On event reason event specific                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to entity is waving from balcony
		*/
		static bool onEventSpecific_WavingFromWindow(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);

		/**
		*  @brief
		*    React to entity is trapped in burning building
		*/
		static bool onEventSpecific_PersonTrappedInBuilding(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity, bool newReason);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
