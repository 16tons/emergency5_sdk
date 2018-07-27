// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/transfer/NetworkTransfer.h"


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
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferReceiverMemory : public NetworkTransfer
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				NetworkTransferReceiverMemory(TransferProtocol& protocol, NetworkTransferId transferId, const std::string& file);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransferReceiverMemory();


			//[-----------------------------------------------------------]
			//[ Public virtual qsf::editor::base::NetworkTransfer methods ]
			//[-----------------------------------------------------------]
			public:
				virtual bool isSender() const override;
				virtual bool isCancel() const override;
				virtual bool processPacket(const packet::TransferData& packet) override;
				virtual bool processPacket(const packet::TransferFinishBase& packet) override;
				virtual uint32 getTransferSize() override;
				virtual uint32 getTransferOffset() override;


			//[-------------------------------------------------------]
			//[ Private definitions                                   ]
			//[-------------------------------------------------------]
			private:
				class WriteDataWorkerTask;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				const std::string					 mFile;
				std::vector<char>					 mFileData;
				uint32								 mFileOffset;
				bool								 mIsCancel;
				uint32								 mFileSize;
				std::shared_ptr<WriteDataWorkerTask> mWorkerTask;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
