// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/reflection/object/Object.h>

#include <string>
#include <vector>


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
	*    Freeplay event hint parameters class
	*/
	class EM5_API_EXPORT EventHintParameters : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*  @note:
		*    Nothing to do here
		*/
		EventHintParameters() :
			mHintsAtLowHealthEnergy_Threshold(0.0f),
			mHintsAtLowLifeEnergy_Threshold(0.0f),
			mHintsAtTimer_Delay(0.0f)
		{
		};

		/**
		*  @brief
		*    Destructor
		*  @note:
		*    Nothing to do here
		*/
		virtual ~EventHintParameters(){};


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		std::string	mSupervisorMessageIds;					///< ID of the supervisor message that is displayed to the player as supervisor message
		std::string	mHintAtFailedIds;						///< ID of the hint that is shown if the event fails

		std::string	mHintsAtLowHealthEnergy;				///< Pool of hints at low health energy
		std::string	mHintsAtLowHealthEnergy_SecondPerson;	///< Pool of hints at low life energy if there a multiple perons injured
		float		mHintsAtLowHealthEnergy_Threshold;		///< Threshold of low health hints

		std::string	mHintsAtLowLifeEnergy;					///< Pool of hints at low life energy
		std::string	mHintsAtLowLifeEnergy_SecondPerson;		///< Pool of hints at low life energy if there a multiple perons injured
		float		mHintsAtLowLifeEnergy_Threshold;		///< Threshold of low life hints

		std::string	mHintsAtInjured;						///< Pool of hints if a person is injured
		std::string	mHintsAtInjured_SecondPerson;			///< Pool of hints if a second person is injured

		std::string	mHintsAtVehicleDamaged;					///< Pool of hints if the first vehicle damaged

		std::string	mHintsAfterInfectionIds;				///<´Hints that apply after a person got infected and shows his state to the player

		std::string	mHintsAtTimer;							///< Hint at timer
		float		mHintsAtTimer_Delay;					///< Delay of timer

		std::string	mHintAtHintFireAtBuildingWithPersonIds;	///< ID of the hint that is shown if an building with persons inside catches fire
		std::string	mHintAtHintFireAtBuildingWithWaverIds;	///< ID of the hint that is shown if an building with a waving person inside catches fire


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EventHintParameters)
