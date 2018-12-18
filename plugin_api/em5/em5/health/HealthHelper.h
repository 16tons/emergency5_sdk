// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/HealthComponent.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
}
namespace qsf
{
	class Entity;
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
	*    EMERGENCY 5 health helper class
	*/
	class EM5_API_EXPORT HealthHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit HealthHelper(qsf::Entity& entity);
		explicit HealthHelper(HealthComponent& healthComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~HealthHelper();

		void drownPerson(FreeplayEvent* freeplayEvent);
		bool injurePerson(uint32 injuryId, FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		bool canTreatedByDoctor();
		bool isInjured();
		bool isKneeingInjury();
		bool isLyingInjury();
		void setInvincible(bool invicible);
		void setImmuneToInjurySources(bool injury, bool illness, bool contamination = true);
		void linkToStretcher(const qsf::Entity& stretcherEntity);

		float getLifeEnergyPercentage();

		//[-------------------------------------------------------]
		//[ Injure persons                                        ]
		//[-------------------------------------------------------]
		/**
		*  brief
		*    Use this method to injure a person by an event
		*/
		bool injurePersonByEventById(uint32 injuryId, FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		/**
		*  brief
		*    Injure person using a pre-edited injury
		*/
		bool injurePersonByInjuryAtStartup(FreeplayEvent* freeplayEvent, uint32 fallbackInjuryId = qsf::getUninitialized<uint32>(), bool playCollapseAnimation = true);

		/**
		*  @brief
		*    Set injuries after the health energy of the person was gorged away by fire
		*/
		bool injurePersonByFire(FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		/**
		*  @brief
		*    Set injuries after the person got to the window and waves
		*/
		bool injurePersonBySmoke(FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		/**
		*  @brief
		*    Set injuries after the person is trapped under a debris
		*/
		bool injurePersonByDebris(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set plague injury
		*/
		bool injurePersonByPlague(FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		/**
		*  @brief
		*    Set injuries after the person got into a contamination cloud
		*/
		bool injurePersonByContamination(FreeplayEvent* freeplayEvent, HealthComponent::ContaminationType contaminationType);

		/**
		*  @brief
		*    Set injuries after the person is injured by a hostage taking
		*/
		bool injurePersonByHostageTaking(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set injuries after the person is shot
		*/
		bool injurePersonByShoot(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set injuries after the person is stabbed by knife
		*/
		bool injurePersonByKnife(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set injuries after the person is hit by a baseball bat
		*/
		bool injurePersonByBaseballBat(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set injuries after the person is hit by a stone
		*/
		bool injurePersonByStone(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set injuries after the person is injuried in a brawl
		*/
		bool injurePersonByBrawl(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Set injuries after the person gets a electric shock
		*/
		bool injurePersonByElectricShock(FreeplayEvent* freeplayEvent);

		/**
		*  brief
		*    Use this method to injure a person by a sender
		*
		*  @note
		*    The sender gives information about the reason why the person gets damaged. So specific injuries can be set.
		*
		*  @todo
		*    - TODO(db): Add more specific cases to this method if there are unhandled damage reasons.
		*/
		bool injurePersonBySender(qsf::Component* sender);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity&		mEntity;
		HealthComponent&	mHealthComponent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/health/HealthHelper-inl.h"
