// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>
#include <qsf/base/GetUninitialized.h>


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
			typedef uint32 NetworkPacketId;
			typedef uint32 NetworkTransferId;

			struct TransferInfo
			{
				NetworkTransferId transferId;
				uint32			  checksum;
				std::string		  fileExtension;

				TransferInfo() :
					transferId(getUninitialized<NetworkTransferId>()),
					checksum(0)
				{}
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
