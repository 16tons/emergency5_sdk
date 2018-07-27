// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <string>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class ChatView;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract chat view controller class
		*/
		class QSF_EDITOR_API_EXPORT ChatViewController : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class ChatView;	// Manages the chat view controller


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ChatViewController methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSendMessageToServer(const std::string& message) = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Default constructor
			*/
			inline ChatViewController();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ChatViewController();

			/**
			*  @brief
			*    Called on message received
			*/
			void onMessageReceived(const std::string& message);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ChatView* mChatView;	///< Can be a null pointer, don't destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/view/chat/ChatViewController-inl.h"
