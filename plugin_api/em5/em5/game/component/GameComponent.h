// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>


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
	*    EMERGENCY 5 game component class. Holds information for the map properties
	*/
	class EM5_API_EXPORT GameComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::GameComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GameComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		~GameComponent();

		float getGameSpeed() const;
		void setGameSpeed(float gameSpeed);

		inline bool isInTutorial() const;
		void setInTutorial(bool tutorialRunning);

		bool getFireSimulationActive() const;
		void setFireSimulationActive(bool fireSimulationActive);

		bool getHealthSimulationActive() const;
		void setHealthSimulationActive(bool healthSimulationActive);

		inline bool getDamageSimulationActive() const;
		void setDamageSimulationActive(bool damageSimulationActive);

		// TODO(vs) Not used anymore.
		// I am bit skeptical about the other behavior flags below.
		// I think ai::NavigationComponent isActive should be sufficient for all these use cases and disable all movement
		inline bool getCivilistBehaviorActive() const;
		void setCivilistBehaviorActive(bool civilistBehaviorActive);

		inline bool getCivilPersonMovement() const;
		void setCivilPersonMovement(bool civilPersonMovement);

		inline bool getCivilVehiclesDrive() const;
		void setCivilVehiclesDrive(bool civilVehiclesDrive);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CivilVehiclesStuckCheck, bool, mCivilVehiclesStuckCheck);

		inline bool getUnitAutomatismActive() const;
		void setUnitAutomatismActive(bool unitAutomatismActive);

		inline bool getAutoEnteringOfDoctorActive() const;
		void setAutoEnteringOfDoctorActive(bool active);

		inline bool getShowGroundMarkersForAllOwnUnits() const;
		void setShowGroundMarkersForAllOwnUnits(bool showGroundMarkersForAllOwnUnits);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mInTutorial;
		bool mDamageSystemActive;
		bool mCivilistBehaviorActive;
		bool mCivilPersonMovement;
		bool mCivilVehiclesDrive;
		bool mCivilVehiclesStuckCheck;
		bool mUnitAutomatismActive;
		bool mAutoEnteringOfDoctorActive;
		bool mShowGroundMarkersForAllOwnUnits;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/component/GameComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameComponent)
