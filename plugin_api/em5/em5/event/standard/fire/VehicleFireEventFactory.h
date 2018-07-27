// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    EMERGENCY 5 freeplay event factory for "vehicle fire"
	*/
	class EM5_API_EXPORT VehicleFireEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VehicleFireEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VehicleFireEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonDamageChance, float, mPersonDamageChance)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DifficultyTags, const std::string&, mDifficultyTags)


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
		bool spawnDriverPerson(qsf::Entity& vehicleEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mPersonDamage;
		float		mPersonDamageChance;
		std::string mDifficultyTags;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleFireEventFactory)
