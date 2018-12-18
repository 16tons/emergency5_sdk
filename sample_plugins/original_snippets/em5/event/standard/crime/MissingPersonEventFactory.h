// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/factory/FreeplayEventFactory.h"

#include <qsf/base/PropertyHelper.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
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
	*    EMERGENCY 5 freeplay event factory for "missing person"
	*
	*  @note
	*    The event has multiple variants:
	*     1. victim is hidden, must search by dog
	*     2. victim is hidden and dead, must search by dog
	*     3. victim is in water, must search by dog (to extra "dogpoint" on ground), the dog creates a divingspot and rescued by diver
	*/
	class EM5_API_EXPORT MissingPersonEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MissingPersonEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MissingPersonEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(IsVictimDeadPercent, float, mIsVictimDeadPercent)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(UseOnlyWaterVictimPercent, float, mUseOnlyWaterVictimPercent)


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* findSpawnPoint(bool useOnlyWaterVictim) const;
		qsf::Entity* createCandidate(const qsf::Entity& spawnPointEntity) const;
		bool checkSpawnPoint(qsf::Entity& targetEntity, bool useOnlyWaterVictim) const;
		bool checkIfNeededUnitsAreAvailable(bool isVictimDead) const;

		// Return the ID of the connected DogSearchPoint or qsf::getUninitialized<unit64>
		uint64 findDogSearchPointEntityFromSpawn(qsf::Entity& spawnPoolEntity) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float mIsVictimDeadPercent;			///< The chance of finding a dead person
		float mUseOnlyWaterVictimPercent;	///< The chance of use only victims in water


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MissingPersonEventFactory)
