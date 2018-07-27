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
	*    Librocket Buy-/Upgrade formatter for the Title and Cost column
	*
	*  @note
	*    This class adds the functionality to display the title and the cost of a upgrade
	*/
	class BuyUpgradeTitleAndCostFormatter : public Rocket::Controls::DataFormatter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuyUpgradeTitleAndCostFormatter();

		/**
		*  @brief
		*    Destructor
		*/
		~BuyUpgradeTitleAndCostFormatter();


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
