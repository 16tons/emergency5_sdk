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
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferSenderFile : public NetworkTransferSender
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				NetworkTransferSenderFile(TransferProtocol& protocol, NetworkTransferId transferId, const std::string& file);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkTransferSenderFile();


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
				boost::nowide::ifstream mStream;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
