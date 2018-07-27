// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/Operation.h"

#include <qsf/message/MessageConfiguration.h>


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
			*    Trigger message operation class
			*
			*  @remarks
			*    Use with care, for each client should react to the message the same way.
			*    Also, there is no undo functionality.
			*/
			class QSF_EDITOR_BASE_API_EXPORT TriggerMessageOperation : public Operation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::TriggerMessageOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				TriggerMessageOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] messageConfiguration
				*    Message with filters
				*/
				explicit TriggerMessageOperation(const MessageConfiguration& messageConfiguration);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~TriggerMessageOperation();

				/**
				*  @brief
				*    Return the message configuration
				*
				*  @return
				*    The message configuration as string
				*/
				std::string getMessageConfiguration() const;
				void setMessageConfiguration(const std::string& messageConfigurationAsString);


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual bool isReversible() const override;
				virtual size_t getMemoryConsumption() const override;
				virtual void undoImpl() override;
				virtual void redoImpl() override;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				MessageConfiguration mMessageConfiguration;	///< Message configuration to trigger


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::TriggerMessageOperation)
