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
#include <qsf/time/Time.h>


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
	*    Emergency 5 freeplay event factory for "fair bomber" 13.4
	*/
	class EM5_API_EXPORT FairBomberEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FairBomberEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FairBomberEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombTime, qsf::Time, mBombTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombDefuseTime, qsf::Time, mBombDefuseTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EscapeTime, qsf::Time, mEscapeTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombSpotTag, const std::string&, mBombSpotTag);


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
		qsf::Entity* findBombSpot(qsf::Entity& candidate) const;
		bool checkBombSpot(qsf::Entity& candidate, qsf::Entity& bombSpot) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time	mBombTime;
		qsf::Time	mBombDefuseTime;
		qsf::Time	mEscapeTime;
		std::string	mBombSpotTag;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FairBomberEventFactory)
