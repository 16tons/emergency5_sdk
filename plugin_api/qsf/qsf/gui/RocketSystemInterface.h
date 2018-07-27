// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Core/SystemInterface.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF system interface for libRocket
	*/
	class RocketSystemInterface : public Rocket::Core::SystemInterface
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RocketSystemInterface();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RocketSystemInterface();


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Core::SystemInterface methods  ]
	//[-------------------------------------------------------]
	public:
		virtual float GetElapsedTime() override;
		virtual int TranslateString(Rocket::Core::String& translated, const Rocket::Core::String& input) override;
		virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message) override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
