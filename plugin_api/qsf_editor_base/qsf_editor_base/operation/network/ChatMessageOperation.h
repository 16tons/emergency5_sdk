// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/network/NetworkOperation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Chat message operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT ChatMessageOperation : public NetworkOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::ChatMessageOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				ChatMessageOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] userName
				*    UTF-8 name of the user who send the chat message
				*  @param[in] message
				*    UTF-8 chat message
				*/
				ChatMessageOperation(const std::string& userName, const std::string& message);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~ChatMessageOperation();

				/**
				*  @brief
				*    Return the name of the user who send the chat message
				*
				*  @return
				*    The UTF-8 name of the user who send the chat message, do not destroy the instance
				*/
				const std::string& getUserName() const;
				void setUserName(const std::string& userName);

				/**
				*  @brief
				*    Return the chat message
				*
				*  @return
				*    The UTF-8 chat message, do not destroy the instance
				*/
				const std::string& getMessage() const;
				void setMessage(const std::string& message);


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual bool isReversible() const override;
				virtual size_t getMemoryConsumption() const override;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::string mUserName;	///< UTF-8 name of the user who send the chat message
				std::string mMessage;	///< UTF-8 chat message


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::ChatMessageOperation)
