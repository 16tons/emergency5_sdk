// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Value;
}
namespace qsf
{
	class MessageParameters;
}


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
	*    Message response receiver class
	*/
	class QSF_API_EXPORT MessageResponseReceiver
	{


	//[-------------------------------------------------------]
	//[ Public qsf::MessageResponseReceiver methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void onMessageResponseReceived(const MessageParameters& parameters, const camp::Value& response) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void registerAsResponseReceiver(MessageParameters& parameters);
		void setMessageResponse(const MessageParameters& parameters, const camp::Value& response);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
