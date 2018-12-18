// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/observer/Observer.h"

#include <qsf/job/JobProxy.h>


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
	*    EMERGENCY 5 logic hint observer person has low energy
	*
	*  @note
	*    - Use the initialize method to configure the hint observer
	*    - This observer observes the energy of its type of an entity in regular updates
	*    - If the energy is low, it sends a message to the general hint low energy observer
	*/
	class EM5_API_EXPORT HintLowEnergyObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::HintLowEnergyObserver" game logic type ID

		enum HintEnergyType
		{
			HINTENERGYTYPE_LIFE,
			HINTENERGYTYPE_HEALTH,
			HINTENERGYTYPE_DAMAGE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HintLowEnergyObserver();

		/**
		*  @brief
		*    Set all required properties
		*/
		void initialize(float energyThreshold, HintEnergyType energyType);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateObserver(const qsf::JobArguments& jobArguments);
		bool checkEnergy();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float			mThresholdEnergy;	///< Value that the energy must fall, so that the hint triggers
		HintEnergyType	mEnergyType;		///< Type of energy the observer observes
		qsf::JobProxy	mUpdateJobProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HintLowEnergyObserver)
