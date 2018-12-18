// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Time;
	class StringHash;
	class MessageConfiguration;
}
namespace em5
{
	class EventHintParameters;
	class FreeplayEvent;
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
	*    Atmo helper can trigger atmo hints
	*/
	class EM5_API_EXPORT AtmosphericRadioMessageHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		// Helper functions for atmo hints
		static void triggerAtmoHintExplosion(qsf::Entity& target);
		static void triggerAtmoHintVehicleUnitDamage(qsf::Entity& target);
		static void triggerAtmoHintPersonUnitDamage(qsf::Entity& target);
		static void triggerAtmoHintThunder();
		static void triggerAtmoHintRain();
		static void triggerAtmoHintFog();
		static void triggerAtmoHintStorm();
		static void clear();


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void triggerAtmoHint(const std::string& hintName, float delay);
		static void playAtmoHint(const std::string& hint);
		static bool checkHintConditions(qsf::Entity* target);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static std::string mLastHint;
		static qsf::JobProxy mSimulationJobProxy;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		AtmosphericRadioMessageHelper();
		~AtmosphericRadioMessageHelper();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
