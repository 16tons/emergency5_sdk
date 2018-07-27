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

#include <boost/nowide/fstream.hpp>


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
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferReceiver : public NetworkTransfer
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				NetworkTransferReceiver(TransferProtocol& protocol, NetworkTransferId transferId, const std::string& file);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransferReceiver();


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
				boost::nowide::ofstream				 mStream;
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
