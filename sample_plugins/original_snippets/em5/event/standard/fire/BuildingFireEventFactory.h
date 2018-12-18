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
#include <qsf/math/EnumBitmask.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingComponent;
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
	*    EMERGENCY 5 freeplay event factory for "building fire event"
	*/
	class EM5_API_EXPORT BuildingFireEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class SearchFlag
		{
			PERSONS_AT_WINDOW,		///< If set, the event can trigger if persons are at the window
			PERSONS_INSIDE,			///< If set, the event can trigger if persons inside the building
			INDUSTRIAL_PLANT,		///< If set, the event can only target industrial plants
			DEFECTIVE_EBOX,			///< If set, the event can only target buildings with attached Eboxes
			EVACUATE_NEIGHBOR_HOUSE	///< If set, the event can only target buildings with a neighbor building to evacuate
		};
		typedef qsf::EnumBitmask<uint8, SearchFlag> SearchFlags;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static qsf::Entity* findCandidate(const std::string& difficultyTags, const std::string& eventTag, const SearchFlags& searchFlags);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuildingFireEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BuildingFireEventFactory();

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
		bool		mTrappedPersonWithPlague;				///< "true" if the event injure the trapped person with plague
		bool		mExplosion;
		float		mEvacuateNeighborHouseChance;			///< Chance that the neighbor house must be evacuate
		float		mSpawnFromConnectedSpawnpoolsChance;	///< Chance that additional injured persons are spawned from linked spawnpools
		bool		mUseAmbientLayer;						///< "true" if an extra layer should be activated
		std::string mDifficultyTags;

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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingFireEventFactory)
