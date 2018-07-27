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
	*    EMERGENCY 5 freeplay event factory for "riot"
	*/
	class EM5_API_EXPORT RiotEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RiotEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RiotEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PeopleToAttack, uint32, mPeopleToAttack);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MinStartGangsters, uint32, mMinStartGangsters);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MaxStartGangsters, uint32, mMaxStartGangsters);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PrimaryBuildingTag, const std::string&, mPrimaryBuildingTag);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SecondaryBuildingTag, const std::string&, mSecondaryBuildingTag);
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
		qsf::Entity* findCandidate() const;
		bool checkCandidate(qsf::Entity& candidate, const std::string& tag) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		mPeopleToAttack;
		uint32		mMinStartGangsters;
		uint32		mMaxStartGangsters;
		std::string mPrimaryBuildingTag;
		std::string mSecondaryBuildingTag;
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RiotEventFactory)
