// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
	*    Hint helper can create hint observers
	*/
	class EM5_API_EXPORT HintHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<std::string> StringArray;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get random string of string array
		*
		*  @return
		*    If the string array is filled, return a random string of this array, or "off" string otherwise
		*/
		static std::string getRandomStringOfStringArray(StringArray& stringArray);

		/**
		*  @brief
		*    Get random hint ID out of string of hint IDs
		*/
		static std::string getRandomStringOfIdString(const std::string& idString);

		static void showHint(const std::string& hintId, uint32 playerIndex = qsf::getUninitialized<uint32>(0), qsf::Time blockedTime = qsf::Time::ZERO);
		static void showTextHint(const std::string& hintId, uint32 playerIndex = qsf::getUninitialized<uint32>(0), qsf::Time blockedTime = qsf::Time::ZERO);
		static void showUnitHint(const std::string& unitHintId, uint32 playerIndex = qsf::getUninitialized<uint32>(0), qsf::Time blockedTime = qsf::Time::ZERO);
		static void showSupervisorMessage(const std::string& messageId, bool audioOnly = false);

		/**
		*  @brief
		*    Clears the hint system from any hint not yet played, stops the current running hint and clears the "hint" window
		*/
		static void clearHints();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit HintHelper(FreeplayEvent& freeplayEvent);

		/**
		*  @brief
		*    Destructor
		*/
		~HintHelper();

		/**
		*  @brief
		*    Get the hint parameters of the event
		*/
		EventHintParameters& getHintParameters() const;

		//[-------------------------------------------------------]
		//[ Helper methods                                        ]
		//[-------------------------------------------------------]
		// Create and initialize observers
		void createAndInitializeHintsAtLowHealth(const qsf::Entity& targetEntity) const;
		void createAndInitializeHintsAtLowLife(const qsf::Entity& targetEntity) const;
		void createAndInitializeHintsAtInjured(const qsf::Entity& targetEntity) const;

		// Create and initialize general observers
		void createAndInitializeGeneralHintAtDamagedVehicle() const;
		void createAndInitializeGeneralHintAtInfectedObserver() const;
		void createAndInitializeGeneralHintsAtInjured() const;
		void createAndInitializeGeneralHintsForMessageObserver(const qsf::MessageConfiguration& messageConfiguration, const qsf::Time& timeFrame, const bool messageCreated) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FreeplayEvent& mEvent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
