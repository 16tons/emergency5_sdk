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
	*    Librocket datagrid data formatter
	*
	*  @note
	*    This class adds the functionality to display things like a checkbox inside of a datagrid table
	*/
	class ExpandButtonFormatter : public Rocket::Controls::DataFormatter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ExpandButtonFormatter();

		/**
		*  @brief
		*    Destructor
		*/
		~ExpandButtonFormatter();


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Controls::DataFormatter methods ]
	//[-------------------------------------------------------]
	public:
		virtual void FormatData(Rocket::Core::String& formattedData, const Rocket::Core::StringList& rawData) override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
