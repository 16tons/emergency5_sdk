// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/transfer/NetworkTransferSender.h"


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
			*    TODO(co) Comment me
			*/
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferSenderFileMemory : public NetworkTransferSender
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				NetworkTransferSenderFileMemory(TransferProtocol& protocol, NetworkTransferId transferId, const std::string& file);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransferSenderFileMemory();


			//[-----------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::NetworkTransferSender methods ]
			//[-----------------------------------------------------------]
			protected:
				virtual bool fileRead(char* buffer, uint32 length) override;
				virtual uint32 getFileOffset() override;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::vector<char> mFileData;
				uint32			  mFileOffset;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
