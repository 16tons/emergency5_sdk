// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/standard/fire/BuildingFireEventFactory.h"


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
	*    EMERGENCY 5 freeplay event factory for "medical in building emergency event"
	*/
	class EM5_API_EXPORT MedicalInBuldingEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef BuildingFireEventFactory::SearchFlag SearchFlag;
		typedef BuildingFireEventFactory::SearchFlags SearchFlags;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MedicalInBuldingEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedicalInBuldingEventFactory();

		QSF_DEFINE_FLAG_GETTER_AND_SETTER(PersonsAtWindow, mSearchFlags, SearchFlag::PERSONS_AT_WINDOW);
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(PersonsInside, mSearchFlags, SearchFlag::PERSONS_INSIDE);
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(IndustrialPlant, mSearchFlags, SearchFlag::INDUSTRIAL_PLANT);
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(DefectEbox, mSearchFlags, SearchFlag::DEFECTIVE_EBOX);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedPersonWithPlague, bool, mTrappedPersonWithPlague);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Explosion, bool, mExplosion);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DifficultyTags, const std::string&, mDifficultyTags);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EvacuateNeighborHouseChance, float, mEvacuateNeighborHouseChance)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SpawnFromConnectedSpanwpoolsChance, float, mSpawnFromConnectedSpawnpoolsChance)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(UseAmbientLayer, bool, mUseAmbientLayer)


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SearchFlags mSearchFlags;
		bool mTrappedPersonWithPlague;				///< "true" if the event injure the trapped person with plague
		bool mExplosion;
		float mEvacuateNeighborHouseChance;			///< Chance that the neighbor house must be evacuate
		float mSpawnFromConnectedSpawnpoolsChance;	///< Chance that additional injured persons are spawned from linked spawnpools
		std::string mDifficultyTags;
		bool mUseAmbientLayer;						///< "true" if an extra layer should be activated

		// Internal
		std::string mLayerName;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MedicalInBuldingEventFactory)
