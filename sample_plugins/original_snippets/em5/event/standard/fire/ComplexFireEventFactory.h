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
	*    EMERGENCY 5 freeplay event factory for "complex fire event"
	*/
	class EM5_API_EXPORT ComplexFireEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ComplexFireEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComplexFireEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonDamageChance, float, mPersonDamageChance)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonMissingChance, float, mPersonMissingChance)
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
		qsf::Entity* getMetaEntity() const;
		bool checkEntity(qsf::Entity& entity) const;
		bool checkPersonSpawnEntity(qsf::Entity& entity) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float		mPersonDamageChance;
		float		mPersonMissingChance;
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ComplexFireEventFactory)
