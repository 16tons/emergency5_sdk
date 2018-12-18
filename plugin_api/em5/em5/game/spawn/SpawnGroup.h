// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/forward/BoostPTree.h>
#include <qsf/math/function/LinearGradientFunction.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CivilistSpawnPointComponent;
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
	*     Spawn group class
	*/
	class EM5_API_EXPORT SpawnGroup : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SpawnGroupManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct LimitDependencyProperties
		{
			float mHourOfDay;
			float mRainIntensity;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		explicit SpawnGroup(uint32);

		/**
		*  @brief
		*    Destructor
		*/
		~SpawnGroup();

		const qsf::NamedIdentifier& getId() const;

		bool loadFromBoostPTree(const boost::property_tree::ptree& pTree);

		void updateCurrentLimit(const LimitDependencyProperties& properties);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setId(const qsf::NamedIdentifier& id);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		qsf::NamedIdentifier mId;
		uint32				 mDefaultLimit;		///< Not necessarily the current limit
		qsf::LinearGradientFunction<float> mSpawnRateByHour;
		qsf::LinearGradientFunction<float> mSpawnRateByRainIntensity;

		// Current state
		uint32 mCurrentNumber;
		uint32 mCurrentLimit;
		std::vector<CivilistSpawnPointComponent*> mSpawnPoints;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
