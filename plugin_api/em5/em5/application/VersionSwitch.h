// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GuiDocument;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/** Version switch class */
	class EM5_API_EXPORT VersionSwitch
	{
	public:
		enum class Edition : unsigned char
		{
			STANDARD = 0,		///< EMERGENCY 5 Standard Edition
			DELUXE,				///< EMERGENCY 5 Deluxe Edition
			EMERGENCY2016,		///< EMERGENCY 2016 (working title: "EM5x"/"Ext01")
			EMERGENCY2017,		///< EMERGENCY 2017
			EMERGENCY20YEARS	///< EMERGENCY 20 years edition
		};

		enum class Distribution : unsigned char
		{
			DIRECT = 0,
			DVD,
			ESD,
			STEAM,
			APPSTORE
		};

	public:
		static const char* getApplicationName();
		static const char* getApplicationSlug();
		static const char* getApplicationVersion();
		static const char* getMultiplayCompatibilityVersion();

		static Edition getEdition();
		static const char* getEditionIdentifier();
		static bool isDeluxe();
		static bool isDeluxeOrHigher();
		static bool isEmergency2016OrHigher();
		static bool isEmergency2017OrHigher();
		static bool isEmergency20YearsOrHigher();

		static Distribution getDistribution();
		static const char* getDistributionIdentifier();
		static void setAutoUpdaterEnabled(bool enabled);
		static bool useAutoUpdater();

		static uint32 getNumberOfCampaigns();

		static void setGuiEditionLogo(qsf::GuiDocument& guiDocument, const std::string& elementName);
		static void setGuiEditionMainMenuBackground(qsf::GuiDocument& guiDocument, const std::string& elementName);
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
