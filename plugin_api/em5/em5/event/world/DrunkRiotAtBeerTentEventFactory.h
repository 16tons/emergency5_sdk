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
	*    Emergency 5 freeplay event factory for "drunk riot at beer tent" 13.1
	*/
	class EM5_API_EXPORT DrunkRiotAtBeerTentEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DrunkRiotAtBeerTentEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DrunkRiotAtBeerTentEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PeopleToAttack, uint32, mPeopleToAttack);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TriggerFromHour, uint32, mHourMin);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TriggerToHour, uint32, mHourMax);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MinStartGangsters, uint32, mMinStartGangsters);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MaxStartGangsters, uint32, mMaxStartGangsters);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FleeThreshold, uint32, mFleeThreshold);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TentBuildingTag, const std::string&, mTentBuildingTag);


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
		bool checkCandidate(qsf::Entity& candidate, const std::string& tag) const;
		bool checkTime() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		mPeopleToAttack;
		uint32		mMinStartGangsters;
		uint32		mMaxStartGangsters;
		uint32		mFleeThreshold;
		std::string mTentBuildingTag;
		uint32		mHourMin;
		uint32		mHourMax;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DrunkRiotAtBeerTentEventFactory)
