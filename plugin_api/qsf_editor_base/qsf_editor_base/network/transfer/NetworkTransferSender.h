// Copyright (C) 2012-2018 Promotion Software GmbH


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
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferSender : public NetworkTransfer
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				NetworkTransferSender(TransferProtocol& protocol, NetworkTransferId transferId);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransferSender();


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


			//[-----------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::NetworkTransferSender methods ]
			//[-----------------------------------------------------------]
			protected:
				virtual bool fileRead(char* buffer, uint32 length) = 0;
				virtual uint32 getFileOffset() = 0;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				void init(uint32 fileSize);
				std::vector<char> read();


			//[-------------------------------------------------------]
			//[ Private definitions                                   ]
			//[-------------------------------------------------------]
			private:
				class ReadDataWorkerTask;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				bool   mIsCancel;
				uint32 mFileSize;
				uint32 mBufferDataOffset;
				std::shared_ptr<ReadDataWorkerTask> mWorkerTask;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
