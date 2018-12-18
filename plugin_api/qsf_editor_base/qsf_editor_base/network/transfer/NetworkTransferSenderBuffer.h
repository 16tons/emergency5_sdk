// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/transfer/NetworkTransferSender.h"

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <vector>
#include <memory>


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
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferSenderBuffer : public NetworkTransferSender
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				NetworkTransferSenderBuffer(TransferProtocol& protocol, NetworkTransferId transferId, std::shared_ptr<const std::vector<char>> buffer);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransferSenderBuffer();


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
				std::shared_ptr<const std::vector<char>> mBuffer;
				uint32 mPosition;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
