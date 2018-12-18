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

#include <boost/noncopyable.hpp>
#include <boost/crc.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Packet;
			class TransferProtocol;
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
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Abstract asset transfer base class
			*/
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransfer : public boost::noncopyable
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 FRAME_SIZE;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransfer();

				NetworkTransferId getTransferId() const;

				const std::string& getFileExtension() const;
				void setFileExtension(const std::string& fileExtension);


			//[-----------------------------------------------------------]
			//[ Public virtual qsf::editor::base::NetworkTransfer methods ]
			//[-----------------------------------------------------------]
			public:
				virtual bool isSender() const = 0;
				virtual bool isCancel() const = 0;
				virtual bool processPacket(const packet::TransferData& inPacket) = 0;
				virtual bool processPacket(const packet::TransferFinishBase& inPacket) = 0;
				virtual uint32 getTransferSize() = 0;
				virtual uint32 getTransferOffset() = 0;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Constructor
				*/
				NetworkTransfer(TransferProtocol& protocol, NetworkTransferId transferId);

				void processFileData(const char* ptr, size_t len);

				uint32 getLocalChecksum();


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				TransferProtocol& mProtocol;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				NetworkTransferId mTransferId;
				std::string mFileExtension;
				boost::crc_32_type mLocalChecksum;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
