// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CommandableComponent;
	class Player;
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
	*    EMERGENCY 5 unit helper class
	*/
	class EM5_API_EXPORT UnitHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit UnitHelper(qsf::Entity& entity);
		~UnitHelper();

		/**
		*  @brief
		*    Returns if the unit contains a injured person
		*/
		bool containsInjuredPersons();

		/**
		*  @brief
		*    Returns if the unit contains at least one gangster
		*/
		bool containsGangster();

		/**
		*  @brief
		*    Returns if the unit is a ambulance NEF
		*/
		bool isAmbulanceNEF() const;
		/**
		*  @brief
		*    Returns if the unit is a ambulance RTW
		*/
		bool isAmbulanceRTW() const;
		/**
		*  @brief
		*    Returns if the unit is a ambulance IMT
		*/
		bool isAmbulanceIMT() const;
		/**
		*  @brief
		*    Returns if the unit is a ambulance Helicopter
		*/
		bool isAmbulanceHelicopter() const;
		/**
		*  @brief
		*    Returns if the unit is a Doctor
		*/
		bool isAmbulanceDoctor() const;
		/**
		*  @brief
		*    Returns if the unit is a Paramedics
		*/
		bool isAmbulanceParamedics() const;
		/**
		*  @brief
		*    Returns if the unit is a ambulance road vehicle
		*/
		bool isAmbulanceRoadVehicle() const;
		/**
		*  @brief
		*    Returns if the unit is a ambulance rescue dog leader
		*/
		bool isAmbulanceRescueDogLeader() const;
		/**
		*  @brief
		*    Returns if the unit is a ambulance rescue dog
		*/
		bool isAmbulanceRescueDog() const;
		/**
		*  @brief
		*    Returns if the unit is a police STW
		*/
		bool isPoliceSTW() const;
		/**
		*  @brief
		*    Returns if the unit is a police MTW
		*/
		bool isPoliceMTW() const;
		/**
		*  @brief
		*    Returns if the unit is a police SEK helicopter
		*/
		bool isPoliceSEKHeli() const;
		/**
		*  @brief
		*    Returns if the unit is a police SEK robot transport vehicle
		*/
		bool isPoliceTSEKR() const;
		/**
		*  @brief
		*    Returns if the unit is a police SEK robot
		*/
		bool isPoliceSEKR() const;
		/**
		*  @brief
		*    Returns if the unit is a police Dog transport car
		*/
		bool isPoliceDogCar() const;
		/**
		*  @brief
		*    Returns if the unit is a police road vehicle
		*/
		bool isPoliceRoadVehicle() const;
		/**
		*  @brief
		*    Returns if the unit is a police Person
		*/
		bool isPolicePerson() const;
		/**
		*  @brief
		*    Returns if the unit is a police SEK Person
		*/
		bool isPoliceSEKPerson() const;
		/**
		*  @brief
		*    Returns if the unit is a police Dog
		*/
		bool isPoliceDog() const;
		/**
		*  @brief
		*    Returns if the unit is a police Person
		*/
		bool isFireFighterRW() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter DECONP
		*/
		bool isFireFighterDECONP() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter DLK
		*/
		bool isFireFighterDLK() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter FLB
		*/
		bool isFireFighterFLB() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter boat transporter
		*/
		bool isFireFighterBoat() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter boat transporter
		*/
		bool isFireFighterBoatTransporter() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter command car
		*/
		bool isFireFighterCommandoCar() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter diver transporter
		*/
		bool isFireFighterDiverTransporter() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter TLF
		*/
		bool isFireFighterTLF() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter road vehicle
		*/
		bool isFireFighterRoadVehicle() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter person
		*/
		bool isFireFighterPerson() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter person and wearing a abc suit
		*/
		bool isFireFighterPersonWithABCSuitActive() const;
		/**
		*  @brief
		*    Returns if the unit is a firefighter drone
		*/
		bool isFireFighterDrone() const;
		/**
		*  @brief
		*    Returns if the unit is a tech engineer car
		*/
		bool isTechEngineerCar() const;
		/**
		*  @brief
		*    Returns if the unit is a tech towcar
		*/
		bool isTechTowcar() const;
		/**
		*  @brief
		*    Returns if the unit is a tech crane
		*/
		bool isTechCrane() const;
		/**
		*  @brief
		*    Returns if the unit is a historical doctor
		*/
		bool isMedievalDoctor() const;
		/**
		*  @brief
		*    Returns if the unit is a historical guardsman
		*/
		bool isMedievalGuardsman() const;

		bool isUnit(const qsf::NamedIdentifier& unitName) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity&			mEntity;
		CommandableComponent*	mCommandableComponent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
