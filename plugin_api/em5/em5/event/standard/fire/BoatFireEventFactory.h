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
	class Layer;
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
	*    EMERGENCY 5 freeplay event factory for "boat fire event"
	*
	*  @remarks
	*    This event has to ways to create:
	*      1. is via (parking) ships in the world
	*      2. is via a Event layer how gets active
	*/
	class EM5_API_EXPORT BoatFireEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BoatFireEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BoatFireEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsOnDeckChance, float, mPersonsOnDeckChance);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ShipOnFireChance, float, mShipOnFireChance);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(UseOnlyShipsFromLayer, bool, mUseOnlyShipsFromLayer);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DifficultyTags, const std::string&, mDifficultyTags);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* findCandidate() const;
		bool checkCandidate(qsf::Entity& entity) const;
		bool isBigShip(qsf::Entity& entity) const;
		bool isBigShip(qsf::Layer& eventLayer);
		bool checkIfNeededUnitsAreAvailable() const;

		bool checkCandidateLayer(const qsf::Layer& layer) const;
		qsf::Layer* findCandidateLayer() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string	mDifficultyTags;
		float		mPersonsOnDeckChance;
		float		mShipOnFireChance;
		bool		mUseOnlyShipsFromLayer;
		std::string	mBaseLayer;
		std::string	mEventLayer;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BoatFireEventFactory)
