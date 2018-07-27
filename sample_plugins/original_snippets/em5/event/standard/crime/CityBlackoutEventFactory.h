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
	*    EMERGENCY 5 freeplay event factory for "City blackout event"
	*/
	class EM5_API_EXPORT CityBlackoutEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CityBlackoutEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CityBlackoutEventFactory();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EventLayer, const std::string&, mEventLayer)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BaseLayer, const std::string&, mBaseLayer)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MinTriggerTimeOfDay, qsf::Time, mMinTriggerTimeOfDay);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MaxTriggerTimeOfDay, qsf::Time, mMaxTriggerTimeOfDay);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Layer* findCandidate() const;
		bool checkCandidate(const qsf::Layer& entity) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mEventLayer;
		std::string mBaseLayer;
		qsf::Time	mMinTriggerTimeOfDay;
		qsf::Time	mMaxTriggerTimeOfDay;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CityBlackoutEventFactory)
