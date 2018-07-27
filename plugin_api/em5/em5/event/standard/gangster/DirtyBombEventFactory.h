// Copyright (C) 2012-2014 Promotion Software GmbH


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
	*    EMERGENCY 5 freeplay event factory for "dirty bomb"
	*/
	class EM5_API_EXPORT DirtyBombEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DirtyBombEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DirtyBombEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(NumberOfTerrorists, uint16, mNumberOfTerrorists);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombTargetTag, const std::string&, mBombTargetTag);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool findTerrorists(std::vector<uint64>& terroristIds) const;
		qsf::Entity* findBombTarget() const;
		bool checkBombTarget(qsf::Entity& bombTarget) const;
		bool checkSpawnPoint(qsf::Entity& spawn) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint16		mNumberOfTerrorists;
		std::string	mBombTargetTag;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DirtyBombEventFactory)
