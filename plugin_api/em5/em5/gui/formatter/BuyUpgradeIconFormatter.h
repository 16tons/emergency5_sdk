// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Controls/DataFormatter.h>


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
	*    Librocket Buy-/Upgrade formatter for the icon
	*
	*  @note
	*    This class adds the functionality to display the icon and info button
	*/
	class BuyUpgradeIconFormatter : public Rocket::Controls::DataFormatter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuyUpgradeIconFormatter();

		/**
		*  @brief
		*    Destructor
		*/
		~BuyUpgradeIconFormatter();


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Controls::DataFormatter methods ]
	//[-------------------------------------------------------]
	public:
		virtual void FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data) override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
