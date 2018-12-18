// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/NetworkTypes.h"

#include <qsf/network/layered/QsfBinaryProtocol.h>

#include <boost/signals2/signal.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class NetworkTransfer;
			class NetworkTransferSender;
			namespace packet
			{
				class TransferData;
				class TransferFinishBase;
			}
		}
	}
}


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
			//[ Type definitions                                      ]
			//[-------------------------------------------------------]
			typedef boost::container::flat_map<NetworkTransferId, NetworkTransfer*> TransferMap;


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Abstract editor asset protocol class
			*/
			class QSF_EDITOR_BASE_API_EXPORT TransferProtocol : public qsf::QsfBinaryProtocol
			{


				friend NetworkTransferSender;


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 TRANSFER_CLIENT_MIN_ID;
				static const uint32 TRANSFER_CLIENT_MAX_ID;
				static const uint32 TRANSFER_SERVER_MIN_ID;
				static const uint32 TRANSFER_SERVER_MAX_ID;

				typedef boost::signals2::signal<void(NetworkTransferId transferId)> TransferStartedSignal;
				typedef boost::signals2::signal<void(NetworkTransferId transferId, uint64 current, uint64 total)> TransferProgressSignal;
				typedef boost::signals2::signal<void(NetworkTransferId transferId, const std::string& fileExtension)> TransferFinishedSignal;
				typedef boost::signals2::signal<void(NetworkTransferId transferId)> TransferCanceledSignal;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				TransferProtocol(QsfProtocol* parent, bool isServer);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~TransferProtocol();


			//[-------------------------------------------------------]
			//[ Boost signals                                         ]
			//[-------------------------------------------------------]
			public: // signals
				//void transferStarted(AssetTransferId transferId);
				TransferStartedSignal transferStarted;
				//void transferProgress(AssetTransferId transferId, uint64 current, uint64 total);
				TransferProgressSignal transferProgress;
				//void transferFinished(AssetTransferId transferId, const std::string& fileExtension);
				TransferFinishedSignal transferFinished;
				//void transferCanceled(AssetTransferId transferId);
				TransferCanceledSignal transferCanceled;


			//[---------------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::TransferProtocol methods ]
			//[---------------------------------------------------------------]
			protected:
				virtual packet::TransferFinishBase* createTransferFinishPacket(NetworkTransferId transferId);


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				NetworkTransferId initTransfer(const std::string& path, bool isSender);
				NetworkTransferId initTransfer(const std::string& path, bool isSender, NetworkTransferId transferId);
				NetworkTransferId initTransfer(std::shared_ptr<const std::vector<char>> buffer, bool isSender, NetworkTransferId transferId);
				void setTransferFileExtension(NetworkTransferId transferId, const std::string& fileExtension);
				void cancelTransfer(NetworkTransferId transferId);
				void processTransferPacket(const packet::TransferData& packet);
				void processTransferPacket(const packet::TransferFinishBase& packet);
				inline bool isServer() const;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				bool              mIsServer;
				TransferMap       mTransfers;
				NetworkTransferId mNextTransferId;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/network/TransferProtocol-inl.h"
