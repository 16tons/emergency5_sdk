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
	*    Achievement icon formatter
	*/
	class AchievementIconFormatter : public Rocket::Controls::DataFormatter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		AchievementIconFormatter();
		~AchievementIconFormatter();


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
