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
	*/
	class EM5_API_EXPORT HeistEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HeistEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HeistEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeistDuration, float, mHeistDuration);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BuildingTag, const std::string&, mBuildingTag);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VictimPool, const std::string&, mVictimPool);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DebugTarget, bool, mDebugTarget);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		std::pair<qsf::Entity*, qsf::Entity*> findCandidate() const;
		qsf::Entity* findBuilding(qsf::Entity& candidate) const;
		bool checkBuilding(qsf::Entity& candidate, qsf::Entity& victim) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float		mHeistDuration;
		std::string mBuildingTag;
		std::string mVictimPool;
		bool		mDebugTarget;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HeistEventFactory)
