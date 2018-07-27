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
	*    Title name formatter
	*/
	class TitleNameFormatter : public Rocket::Controls::DataFormatter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		TitleNameFormatter();
		~TitleNameFormatter();


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
