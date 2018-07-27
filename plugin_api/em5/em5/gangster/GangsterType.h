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
#include <qsf/forward/BoostPTree.h>
#include <qsf/reflection/object/Object.h>


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
	*    Gangster type definition class
	*/
	class EM5_API_EXPORT GangsterType : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef GangsterBaseLogic::AutoReaction AutoReaction;
		typedef GangsterBaseLogic::WeaponFlagSet WeaponFlagSet;
		typedef weapon::WeaponConfiguration WeaponConfiguration;
		typedef boost::container::flat_map<qsf::StringHash, uint32> PriorityByEventTagMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GangsterType(uint32 key);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GangsterType();

		// Get the gangster type ID
		uint32 getId() const;

		// Get the name of the gangster type
		const std::string& getName() const;

		// Set the name of the gangster type
		void setName(const std::string& name);

		// Get the gangster logic class ID
		uint32 getLogicClassId() const;

		// Get the gangster logic class name
		const std::string& getLogicClassName() const;

		// Set the gangster logic class name
		void setLogicClassName(const std::string& className);

		//[-------------------------------------------------------]
		//[ Weapons                                               ]
		//[-------------------------------------------------------]
		// Get equipped weapons
		const WeaponFlagSet& getWeapons() const;

		// Get equipped weapons as comma-separated string
		std::string getWeaponsAsString() const;

		// Set equipped weapons from a comma-separated string
		void setWeaponsByString(const std::string& weapons);

		// Property "AlwaysShowWeapon"
		bool getAlwaysShowWeapon() const;
		void setAlwaysShowWeapon(bool showWeapon);

		// Weapon configuration (firearms only)
		WeaponConfiguration* getConfigurationForWeapon(weapon::Weapon weapon);
		const WeaponConfiguration* getConfigurationForWeapon(weapon::Weapon weapon) const;

		// Firearms shoot range
		float getShootRangePistol() const;
		void setShootRangePistol(float range);
		float getShootRangeRifle() const;
		void setShootRangeRifle(float range);

		// Firearms damage to persons
		float getDamageHealthPistol() const;
		void setDamageHealthPistol(float damage);
		float getDamageHealthRifle() const;
		void setDamageHealthRifle(float damage);

		// Firearms damage to vehicles
		float getDamageVehiclePistol() const;
		void setDamageVehiclePistol(float damage);
		float getDamageVehicleRifle() const;
		void setDamageVehicleRifle(float damage);

		//[-------------------------------------------------------]
		//[ Behavior                                              ]
		//[-------------------------------------------------------]
		// Property "MovementSpeed"
		float getMovementSpeed() const;
		void setMovementSpeed(float movementSpeed);

		// Property "ResistArrest"
		bool getResistArrest() const;
		void setResistArrest(bool resistArrest);

		// Property "CelebrateWinTime"
		float getCelebrateWinTime() const;
		void setCelebrateWinTime(float celebrateWinTime);

		//[-------------------------------------------------------]
		//[ Auto-aggression                                       ]
		//[-------------------------------------------------------]
		// Property "AutoAggroRange"
		float getAutoAggroRange() const;
		void setAutoAggroRange(float autoAggroRange);

		// Property "AutoDeAggroRange"
		float getAutoDeAggroRange() const;
		void setAutoDeAggroRange(float autoDeAggroRange);

		// Property "AutoTargets"
		const PriorityByEventTagMap& getAutoTargets() const;
		boost::property_tree::ptree getAutoTargetsAsPTree() const;
		void setAutoTargetsByPTree(const boost::property_tree::ptree& pTree);

		// Property "AutoTargetReaction"
		AutoReaction getAutoTargetReaction() const;
		void setAutoTargetReaction(AutoReaction autoTargetReaction);

		// Property "IdleReaction"
		AutoReaction getIdleReaction() const;
		void setIdleReaction(AutoReaction idleReaction);

		// Property "ChainReaction"
		bool getChainReaction() const;
		void setChainReaction(bool chainReaction);

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::NamedIdentifier  mName;					///< Gangster type name; must be unique
		qsf::NamedIdentifier  mLogicClassName;			///< Name and ID of gangster logic CAMP class (e.g. "em5::GangsterBrawlerUnarmed")

		// Weapons
		WeaponFlagSet		  mWeapons;					///< Indicates which weapons the gangster has equipped
		bool				  mAlwaysShowWeapon;		///< If set, equipped weapon is always shown openly
		WeaponConfiguration	  mWeaponConfigurationForPistol;	///< Contains all weapon-specific settings for pistol usage
		WeaponConfiguration	  mWeaponConfigurationForRifle;		///< Contains all weapon-specific settings for rifle usage

		// Behavior
		float				  mMovementSpeed;			///< Speed (meters per second) when moving; set to 0.0f to use default speed
		bool				  mResistArrest;			///< If "false", the gangster will just giving without a fight when being arrest
		qsf::Time			  mCelebrateWinTime;		///< How long will the gangster cheer after a won fight; can be zero

		// Auto-aggression
		float				  mAutoAggroRange;			///< Range this gangster searches for targets
		float				  mAutoDeAggroRange;		///< Range where gangster unit ignored the target
		PriorityByEventTagMap mAutoTargets;				///< Event tags we want to attack, and their priorities
		AutoReaction		  mAutoTargetReaction;
		AutoReaction		  mIdleReaction;
		bool				  mChainReaction;			///< If true, the gangster listen to shoots of nearby gangsters and starts autoreact to attacker


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterType)
