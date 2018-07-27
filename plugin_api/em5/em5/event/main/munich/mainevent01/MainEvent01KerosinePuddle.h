// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
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
	*    EMERGENCY 5 main event 01 growing kerosine puddle
	*/
	class MainEvent01KerosinePuddle
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent01KerosinePuddle();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent01KerosinePuddle();

		/**
		*  @brief
		*    Reset the puddle to the start value
		*/
		void reset();

		/**
		*  @brief
		*    Start to grow the puddle
		*
		*  @param[in] timeToGrow
		*    Time the puddle has to grow to full size
		*/
		void startGrowing(const qsf::Time& timeToGrow);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updatePuddle(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GlobalAssetId	mKerosinId;
		qsf::JobProxy		mUpdateJobProxy;	///< Job proxy
		qsf::Time			mTimePassed;
		qsf::Time			mTimeToGrow;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
